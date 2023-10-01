#pragma once
#include <Arduino.h>
#include <ArduinoLog.h>
#include <CircularBuffer.h>

#include "joystick_descriptor.h"
#include "joystick_global.h"

constexpr uint8_t buffer_size = 8;

uint16_t get_avg_from_buffer(CircularBuffer<uint16_t, buffer_size> &buffer) {
  uint32_t value = 0;
  for (uint8_t i = 0; i < buffer.size(); i++) {
    value += buffer[i];
  }
  value /= buffer.size();
  return value;
}

void adc_task(void *pvParameter) {
  Log.info(F("Native ADC reading task started"));
  analogReadResolution(12);

  // create  task that reads adc example on gpio0..gpio3
  int pins[4] = {1, 2, 3};

  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], INPUT);
  }

  CircularBuffer<uint16_t, buffer_size> buffers[3];
  unsigned long period;
  while (true) {
    auto start_time = millis();
    uint16_t values[3];
    for (int i = 0; i < 3; i++) {
      auto reading = analogRead(pins[i]);
      buffers[i].push(reading);
    }

    CircularBuffer<uint16_t, buffer_size> buffers[3];
    unsigned long period;
    while (true) {
      auto start_time = millis();
      int16_t values[3];
      for (int i = 0; i < 3; i++) {
        auto reading = analogRead(pins[i]);
        buffers[i].push(reading);
      }
      for (int i = 0; i < 3; i++) {
        values[i] = get_avg_from_buffer(buffers[i]);
      }
      // invert adc_chanels[0] values
      values[0] = AXIS_MAX_VALUE - values[0];
      values[1] = AXIS_MAX_VALUE - values[1];
      values[2] = AXIS_MAX_VALUE - values[2];

      // map values to 0..3000
      values[0] = map(values[0], 0, AXIS_MAX_VALUE, 0, 3000);
      values[1] = map(values[1], 0, AXIS_MAX_VALUE, 0, 3000);
      values[2] = map(values[2], 0, AXIS_MAX_VALUE, 0, 3000);

      gamepad.joystick1(values[0], values[1], values[2]);
      // gamepad.joystick1(0,0,0);
      period = millis() - start_time;
      // print period every second
      static unsigned long last_time = 0;
      if (millis() - last_time > 1000) {
        Log.info("adc native period: %d, %d\t%d\t%d\n", period, values[0],
                      values[1], values[2]);
        last_time = millis();
      }
      delay(5);
    }
  }
}