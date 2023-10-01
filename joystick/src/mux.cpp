#include <Arduino.h>
#include "mux.h"

constexpr uint8_t en1 = 14;
constexpr uint8_t en2 = 12;
constexpr uint8_t s0 = 11;
constexpr uint8_t s1 = 13;
constexpr uint8_t s2 = 10;
constexpr uint8_t s3 = 9;
constexpr uint8_t pin = 8;

bool print_mux = false;

void init_mux()
{
    pinMode(pin, INPUT_PULLUP);
    pinMode(en1, OUTPUT);
    pinMode(en2, OUTPUT);
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    digitalWrite(en1, 1);
    digitalWrite(en1, 1);
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
}

uint32_t read_muxes()
{

    uint32_t mux_values = 0;

    // print every 1 second
    // static unsigned long last_time = 0;
    // if (millis() - last_time > 1000)
    // {
    //     print_mux = true;
    //     last_time = millis();
    // }
    // // set first mux to enable
    digitalWrite(en1, 0);
    digitalWrite(en2, 1);
    for (uint8_t i = 0; i < 32; i++)
    {
        // set second mux to enable
        if (i == 16)
        {
            digitalWrite(en1, 1);
            digitalWrite(en2, 0);
        }
        uint8_t number = i >= 16 ? i - 16 : i;
        digitalWrite(s0, number & 0x01);
        digitalWrite(s1, number & 0x02);
        digitalWrite(s2, number & 0x04);
        digitalWrite(s3, number & 0x08);
        // delayMicroseconds(10);
        delay(1);
        uint8_t value = digitalRead(pin) == 0;
        // set bit of channel in mux_values
        mux_values |= (value << i);
    }

    digitalWrite(en1, 1);
    digitalWrite(en2, 1);

    return mux_values;
}