#include <Arduino.h>
#include "mux.h"

constexpr uint8_t en1 = 14;
constexpr uint8_t en2 = 13;
constexpr uint8_t s0 = 12;
constexpr uint8_t s1 = 11;
constexpr uint8_t s2 = 10;
constexpr uint8_t s3 = 9;
constexpr uint8_t pin = 8;

bool print_mux = false;

void init_mux(){
    pinMode(pin, INPUT_PULLUP);
    pinMode(en1, OUTPUT);
    pinMode(en2, OUTPUT);
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
}

void set_mux(uint8_t channel)
{
    uint32_t mask = GPIO.out_w1ts;
    mask |= (1 << channel < 16 ? en1 : en2);
    mask |= (1 << (channel % 16)) << s0;
    mask |= (1 << (channel % 16)) << s1;
    mask |= (1 << (channel % 16)) << s2;
    mask |= (1 << (channel % 16)) << s3;
    GPIO.out_w1ts = mask;
    // print pin values with names
    if (print_mux)
    {
        Serial.printf("en1: %d, en2: %d, s0: %d, s1: %d, s2: %d, s3: %d\n", digitalRead(en1), digitalRead(en2), digitalRead(s0), digitalRead(s1), digitalRead(s2), digitalRead(s3));
    }
}

bool read_mux(uint8_t channel)
{
    set_mux(channel);
    uint8_t value = digitalRead(pin);
    // print values with names
    if (print_mux)
    {
        Serial.printf("mux: %d, channel: %d\n", value, channel);
    }
    return value;
}

uint32_t read_muxes()
{

    uint32_t mux_values = 0;
    for (uint8_t i = 0; i < 32; i++)
    {
        // print every 1 second
        static unsigned long last_time = 0;
        if (millis() - last_time > 1000)
        {
            print_mux = true;
            Serial.println("mux read print -----------");
            last_time = millis();
        }
        mux_values |= (read_mux(i) << i);
        print_mux = false;
    }
    return mux_values;
}