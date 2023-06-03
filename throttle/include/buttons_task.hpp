#include <Arduino.h>
#include "joystick_global.h"

constexpr uint8_t button_pins[8] = {7, 8, 9,10, 11, 12, 13,14};

// create a mux task that reads all 32 channels of mux
void buttons_task(void *pvParameters)
{
    // set pins as inputs pullup
    for (auto p : button_pins)
    {
        pinMode(p, INPUT_PULLUP);
    }

    while (true)
    {
        auto start_time = millis();
        uint8_t buttons = 0;
        // read all buttons and pack into one byte
        for (int i = 0; i < 8; i++)
        {
            if (digitalRead(button_pins[i]) == LOW)
            {
                buttons |= 1 << i;
            }
        }
        gamepad.buttons(buttons);
        auto period = millis() - start_time;

        // print period every second
        static unsigned long last_time = 0;
        if (millis() - last_time > 1000)
        {
            // construct a string from buttons as bits 
            String s;
            for (int i = 0; i < 8; i++)
            {
                s += (buttons & (1 << i)) ? "1" : "0";
            }
            Serial.printf("buttons period: %d, value %s\n", period, s);
            last_time = millis();
        }

        delay(3);
    }
}