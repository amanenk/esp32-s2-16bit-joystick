#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "joystick.h"
#include <Adafruit_NeoPixel.h>
#include "mux.h"
#include "adc_task.hpp"
#include "mux_task.hpp"

#if CFG_TUD_HID
HIDgamepad gamepad;

// On the ESP32S2 SAOLA GPIO is the NeoPixel.
#define PIN 18
// Single NeoPixel
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 25 // Time (in milliseconds) to pause between color change

#include "ADS1X15.h"

#include <CircularBuffer.h>

float f = 0;
constexpr uint32_t multiplier = 10000;

CircularBuffer<double, 10> time_buffer;

void setup()
{
  Serial.begin(115200);
  gamepad.product("mmnk gamepad");
  gamepad.manufacturer("mmnk");
  gamepad.serial("mnnk1234-567890");

  gamepad.begin();
  pixels.setBrightness(10);
  pixels.begin(); // INITIALIZE NeoPixel (REQUIRED)

  // create adc task
  xTaskCreate(adc_task, "adc_task", 4096, NULL, 1, NULL);
  // create mux task
  xTaskCreate(mux_task, "mux_task", 4096, NULL, 1, NULL);
}

unsigned long loop_last_time = 0;
void loop()
{
  auto period = millis() - loop_last_time;
  loop_last_time = millis();

  static unsigned long last_time = 0;
  if (millis() - last_time > 1000)
  {
    Serial.printf("loop_period: %d\n", period);
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
