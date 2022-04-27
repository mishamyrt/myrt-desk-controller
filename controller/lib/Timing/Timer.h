// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>

class Timer {
 public:
  bool isDone() {
    return millis() >= _end;
  }

  void set(size_t ms) {
    _end = millis() + ms;
  }

 private:
  uint32_t _end = 0;
};
