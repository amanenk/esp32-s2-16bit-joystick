#include <Arduino.h>

#include <CircularBuffer.h>
#include "joystick_global.h"

constexpr uint8_t pin = 1;
constexpr uint8_t buffer_size = 30;
// create a task that inits and reads ADS in a loop
void adc_task(void *pvParameter)
{
    analogReadResolution(12);

    // create  task that reads adc example on gpio0..gpio3
    int pins[4] = {1};
    pinMode(pin, INPUT);

    CircularBuffer<uint16_t, buffer_size> buffer;

    unsigned long period;
    while (true)
    {
        auto start_time = millis();
        buffer.push(analogRead(pin));
        // get average value of buffer
        uint32_t value = 0;
        for (uint8_t i = 0; i < buffer.size(); i++)
        {
            value += buffer[i];
        }
        value /= buffer.size();
        gamepad.dial(value);
        // gamepad.joystick1(0,0,0);
        period = millis() - start_time;
        // print period every second
        static unsigned long last_time = 0;
        if (millis() - last_time > 1000)
        {
            Serial.printf("adc native period: %d, %d\t%d\t%d\t%d\n", period, value);
            last_time = millis();
        }
        delay(5);
    }
}