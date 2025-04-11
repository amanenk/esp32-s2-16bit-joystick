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

  int16_t min_values[3] = {4095, 4095, 4095};
  int16_t max_values[3] = {0, 0, 0};

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
    
      int16_t new_values[3];
      for (int i = 0; i < 3; i++) {
        if (values[i] < min_values[i]) {
          min_values[i] = values[i];
        }
        if (values[i] > max_values[i]) {
          max_values[i] = values[i];
        }
        
        new_values[i] = map(values[i], min_values[i], max_values[i], 0, AXIS_MAX_VALUE);
      }

      gamepad.joystick1(new_values[0], new_values[1], new_values[2]);
      // gamepad.joystick1(0,0,0);
      period = millis() - start_time;
      // print period every second
      static unsigned long last_time = 0;
      if (millis() - last_time > 1000) {
        Log.info("read_values: %d\t%d\t%d\n", values[0],
                      values[1], values[2]);
        Log.trace("min values: %d\t%d\t%d\n", min_values[0], min_values[1],
                  min_values[2]);
        Log.trace("max values: %d\t%d\t%d\n", max_values[0], max_values[1],
                  max_values[2]);
        Log.trace("transformed values: %d\t%d\t%d\n", new_values[0],
                  new_values[1], new_values[2]);
        last_time = millis();
      }
      delay(5);
    }
  }
}