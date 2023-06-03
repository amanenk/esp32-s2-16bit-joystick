#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "mux.h"
#include "adc_task.hpp"
#include "mux_task.hpp"
#include "usb_task.hpp"

// #define USE_USB_SERIAL

#ifdef USE_USB_SERIAL
#include "cdcusb.h"
CDCusb USBSerial;
#endif

void setup()
{

#ifdef USE_USB_SERIAL
  if (!USBSerial.begin())
    Serial.println("Failed to start CDC USB stack");
#else
  Serial.begin(115200);
#endif

  delay(1000);
  Serial.printf("setup: start\n");

  init_joystick();

  // create adc task
  xTaskCreate(adc_task, "adc_task", 10000, NULL, 1, NULL);
  // create mux task
  xTaskCreate(mux_task, "mux_task", 4096, NULL, 1, NULL);
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
