#define USE_USB_SERIAL
#pragma once

#ifdef USE_USB_SERIAL
#include "cdcusb.h"
CDCusb myUSBSerial;
// redirect myUSBSerial to Serial
// #define Serial myUSBSerial

#endif

void init_usb_serial()
{
    Serial.begin(115200);
#ifdef USE_USB_SERIAL
    myUSBSerial.begin();
#endif
}