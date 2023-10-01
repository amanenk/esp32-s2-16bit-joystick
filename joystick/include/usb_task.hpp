#include "ArduinoLog.h"
#include "joystick_global.h"

// create a task that inits and reads ADS in a loop
void usb_task(void *pvParameter) {
  Log.info("usb task started\n");  
  init_joystick();

  while (true) {
    gamepad.sendReport();
    delay(10);
  }
}