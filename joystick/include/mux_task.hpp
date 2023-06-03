#include <Arduino.h>
#include "mux.h"
#include "joystick_global.h"

// create a mux task that reads all 32 channels of mux
void mux_task(void *pvParameters)
{
    init_mux();
    while (true)
    {
        auto start_time = millis();
        auto mux_values = read_muxes();
        gamepad.buttons(mux_values);
        auto period = millis() - start_time;

        // print period every second
        static unsigned long last_time = 0;
        if (millis() - last_time > 1000)
        {
            Serial.printf("mux period: %d, value %d\n", period, mux_values);
            last_time = millis();
        }

        delay(3);
    }
}