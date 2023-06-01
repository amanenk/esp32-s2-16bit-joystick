#include <Arduino.h>

#include "joystick_global.h"
// create a task that inits and reads ADS in a loop
void adc_task2(void *pvParameter)
{
    Serial.println("native adc reading test");
    analogReadResolution(12);

    // create  task that reads adc example on gpio0..gpio3
    int pins[4] = {1, 2, 3};

    for (int i = 0; i < 4; i++)
    {
        pinMode(pins[i], INPUT);
    }
    unsigned long period;
    while (true)
    {
        auto start_time = millis();
        int16_t adc_chanels[3] = {0, 0, 0};
        for (int i = 0; i < 3; i++)
        {
            adc_chanels[i] = analogRead(pins[i]);
        }

        int16_t x = map(adc_chanels[0], 0, 4095, -32767, 32767);
        int16_t y = map(adc_chanels[1], 0, 4095, -32767, 32767);
        int16_t z = map(adc_chanels[2], 0, 4095, -32767, 32767);
        gamepad.joystick1(x, y, z);
        // gamepad.joystick1(0,0,0);
        period = millis() - start_time;
        // print period every second
        static unsigned long last_time = 0;
        if (millis() - last_time > 1000)
        {
            Serial.printf("adc native period: %d, %d\t%d\t%d\t%d\n", period, adc_chanels[0], adc_chanels[1], adc_chanels[2], adc_chanels[3]);
            last_time = millis();
        }
        delay(5);
    }
}