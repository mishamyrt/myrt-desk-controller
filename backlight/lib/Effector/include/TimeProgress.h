// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>

class TimeProgress {
 public:
  void setMaxProgress(size_t max_value) {
    _max_value = max_value;
  }

  void setDuration(size_t duration) {
    _duration = duration;
    reset();
  }

  void reset() {
    _start_time = millis();
    _value = 0;
  }

  void tick() {
    _value = constrain(
      map(
        millis() - _start_time,
        0, _duration,
        0, _max_value
      ),
      0, _max_value
    );
  }

  bool is_done() {
    return _value == _max_value;
  }

  size_t value() {
    return _value;
  }

 private:
  uint64_t _start_time = 0;
  size_t _duration = 0;
  size_t _max_value = 255;
  size_t _value;
};
