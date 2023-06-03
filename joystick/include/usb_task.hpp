#include "joystick_global.h"
// create a task that inits and reads ADS in a loop
void usb_task(void *pvParameter)
{
    Serial.println("usb task started");

    while (true)
    {
        gamepad.sendReport();
        delay(10);
    }
}