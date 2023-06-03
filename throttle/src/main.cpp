#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "adc_task.hpp"
#include "buttons_task.hpp"
#include "usb_task.hpp"

// #include "cdcusb.h"
// CDCusb USBSerial;

// #include "webusb.h"
// WebUSB USBSerial;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // if (!USBSerial.begin())
  //       Serial.println("Failed to start CDC USB stack");

  delay(1000);

  Serial.printf("setup: start\n");
  // USBSerial.printf("setup: start\n");

  init_joystick();

  // create adc task
  xTaskCreate(adc_task, "adc_task", 4096, NULL, 1, NULL);
  // create mux task
  xTaskCreate(buttons_task, "mux_task", 4096, NULL, 1, NULL);
  // create mux task
  xTaskCreate(usb_task, "usb_task", 4096, NULL, 2, NULL);
}

void loop()
{
  // print ping from usb serial eveery one second
  static unsigned long last_time = 0;
  if (millis() - last_time > 1000)
  {
    // USBSerial.printf("ping: %lu\n", millis());
    last_time = millis();
  }
}
