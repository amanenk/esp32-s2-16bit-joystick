#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "joystick.h"
#include <Adafruit_NeoPixel.h>
#include "mux.h"

#if CFG_TUD_HID
HIDgamepad gamepad;

// On the ESP32S2 SAOLA GPIO is the NeoPixel.
#define PIN 18
// Single NeoPixel
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 25 // Time (in milliseconds) to pause between color change

#include "ADS1X15.h"

#include <CircularBuffer.h>

ADS1115 ADS(0x48);
float f = 0;
bool adc_ready = false;
uint8_t channel = 0;
int32_t adc_values[4] = {0, 0, 0, 0};
constexpr uint8_t max_channels = 3;
constexpr uint8_t rdy_pin = 7;

CircularBuffer<double, 10> time_buffer;

void adc_ready_calback()
{
  adc_ready = true;
}

void setup()
{
  Serial.begin(115200);
  gamepad.product("mmnk gamepad");
  gamepad.manufacturer("mmnk");
  gamepad.serial("mnnk1234-567890");

  gamepad.begin();
  pixels.setBrightness(10);
  pixels.begin(); // INITIALIZE NeoPixel (REQUIRED)

  ADS.begin();
  ADS.setGain(1);      // 6.144 volt
  ADS.setDataRate(7);  // fast
  ADS.setMode(0);      // continuous mode
  f = ADS.toVoltage(); // voltage factor
  ADS.requestADC(0);
  // SET ALERT RDY PIN
  ADS.setComparatorThresholdHigh(0x8000);
  ADS.setComparatorThresholdLow(0x0000);
  ADS.setComparatorQueConvert(0);

  // SET INTERRUPT HANDLER TO CATCH CONVERSION READY
  pinMode(rdy_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rdy_pin), adc_ready_calback, RISING);

  ADS.readADC(channel); // trigger first read

  // setup mux
  init_mux();
}

// handle conversions if both are ready
bool handleConversion()
{
  if (adc_ready == false)
    return false;

  // read the value of both
  int16_t a = ADS.getValue();
  adc_values[channel] = a;
  channel++;
  if (channel >= max_channels)
    channel = 0;
  ADS.readADC(channel);
  adc_ready = false;

  return true;
}

unsigned long last_time = 0;
void loop()
{
  auto start_time = millis();
  if (handleConversion() == true)
  {
    // Serial.printf("read time: %d, value %d, %d, %d, %d\n", millis() - start_time, adc_values[0], adc_values[1], adc_values[2], adc_values[3]);
    // Serial.printf("read time: %d, value %f, %f, %f, %f\n", millis() - start_time, adc_values[0] * f, adc_values[1] * f, adc_values[2] * f, adc_values[3] * f);
    // Serial.println("last time: " + String(millis() - last_time) + "ms");
    int multiplier = 10000;
    int16_t x = map(f * adc_values[0] * multiplier, 0.8 * multiplier, 4 * multiplier, -32768, 32767);
    int16_t y = map(f * adc_values[1] * multiplier, 0.8 * multiplier, 4 * multiplier, -32768, 32767);
    gamepad.joystick1(x, y, 0);
  }

  static unsigned long last_time = 0;
  if (millis() - last_time > 1000)
  {
    Serial.printf("time: %d, value %f, %f, %f, %f\n", millis() - start_time, adc_values[0] * f, adc_values[1] * f, adc_values[2] * f, adc_values[3] * f);
    last_time = millis();
  }

  auto mux_values = read_muxes();

  // Serial.printf("read time: %d, value %d\n", millis() - start_time, x);

  // Serial.println("loop");
  // // Set the new color on the pixel.
  // pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  // // Send the updated pixel colors to the hardware.
  // pixels.show();

  // int max = 30000;
  // int step = 100;
  // int step_delay = 50;
  // for (int v = -max; v < max; v += step)
  // {
  //   gamepad.sendAll(0, x, v, v, v, v, v, 0);
  //   delay(step_delay);
  // }

  // delay(step_delay * 10);

  // for (int v = max; v > -max; v -= step)
  // {
  //   gamepad.sendAll(0, x, v, v, v, v, v, 0);
  //   delay(step_delay);
  // }

  // // Set the new color on the pixel.
  // pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  // // Send the updated pixel colors to the hardware.
  // pixels.show();

  // delay(1000);
  // delay(100);
}

#endif
