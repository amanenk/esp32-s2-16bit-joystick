#pragma once
#include "ArduinoLog.h"
#include "joystick_global.h"
#include "mux.h"
#include <Arduino.h>


void mux_task(void *pvParameters) {
  init_mux();
  while (true) {
    auto start_time = millis();
    auto mux_values = read_muxes();
    gamepad.buttons(mux_values);
    auto period = millis() - start_time;

    // print period every second
    static unsigned long last_time = 0;
    if (millis() - last_time > 1000) {
      String mux_string = "";
      for (uint8_t i = 0; i < 32; i++) {
        mux_string += String((mux_values >> i) & 1);
      }

      Log.info("mux period: %d, value %s\n", period, mux_string.c_str());

      last_time = millis();
    }

    delay(3);
  }
}
