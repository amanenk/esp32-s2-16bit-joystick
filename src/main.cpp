#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "joystick.h"
#include <Adafruit_NeoPixel.h>

#if CFG_TUD_HID
HIDgamepad gamepad;

// On the ESP32S2 SAOLA GPIO is the NeoPixel.
#define PIN 18
// Single NeoPixel
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 25 // Time (in milliseconds) to pause between color change

void setup()
{
  Serial.begin(115200);
  gamepad.product("mmnk gamepad");
  gamepad.manufacturer("mmnk");
  gamepad.serial("mnnk1234-567890");

  gamepad.begin();
  pixels.setBrightness(10);
  pixels.begin(); // INITIALIZE NeoPixel (REQUIRED)
}

void loop()
{
  Serial.println("loop");
  // Set the new color on the pixel.
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  // Send the updated pixel colors to the hardware.
  pixels.show();

  int max = 30000;
  int step = 100;
  int step_delay = 50;
  for (int v = -max; v < max; v += step)
  {
    gamepad.sendAll(0, v, v, v, v, v, v, 0);
    delay(step_delay);
  }

  delay(step_delay * 10);

  for (int v = max; v > -max; v -= step)
  {
    gamepad.sendAll(0, v, v, v, v, v, v, 0);
    delay(step_delay);
  }

  // Set the new color on the pixel.
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  // Send the updated pixel colors to the hardware.
  pixels.show();

  delay(1000);
}

#endif

// #include "hidgamepad.h"
// #if CFG_TUD_HID
// HIDgamepad gamepad;

// void setup()
// {
//   Serial.begin(115200);
//   gamepad.begin();
// }

// void loop()
// {
//   delay(1000);
//   // 32 buttons
//   for (size_t i = 0; i < 32; i++)
//   {
//     // buttons send map of buttons represented by bits
//     gamepad.buttons(1 << i);
//     delay(100);
//   }

//   // hat 8 positions
//   for (size_t i = 0; i < 9; i++)
//   {
//     gamepad.hat(i);
//     delay(100);
//   }

//   gamepad.sendAll(0, 0, 0, 0, 0, 0, 0, 0);
//   // x, y, z
//   gamepad.joystick1(100, -100, 50);
//   delay(1000);
//   gamepad.joystick1(-100, 100, -50);
//   delay(1000);
//   // Rx, Ry, Rz
//   gamepad.joystick2(100, -100, 50);
//   delay(1000);
//   gamepad.joystick2(-100, 100, -50);
//   delay(1000);
//   // Button Map |  X | Y | Z | Rx | Ry | Rz | hat
//   gamepad.sendAll(0xffffffff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 5);
//   delay(1000);
//   gamepad.sendAll(0, 0, 0, 0, 0, 0, 0, 0);
//   Serial.println("loop");
// }

// #endif
