#include <Arduino.h>
#include "mux.h"

constexpr uint8_t en1 = 11;
constexpr uint8_t en2 = 12;
constexpr uint8_t s0 = 13;
constexpr uint8_t s1 = 14;
constexpr uint8_t s2 = 15;
constexpr uint8_t s3 = 16;
constexpr uint8_t pin = 17;

void init_mux()
{
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
}

bool read_mux(uint8_t channel)
{
    set_mux(channel);
    return digitalRead(pin);
}

uint32_t read_muxes()
{
    uint32_t mux_values = 0;
    for (uint8_t i = 0; i < 32; i++)
    {
        mux_values |= (read_mux(i) << i);
    }
    return mux_values;
}