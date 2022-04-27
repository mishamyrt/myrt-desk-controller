// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>

class Stopwatch {
 public:
  void start() {
    _start = millis();
  }

  size_t split() {
    return millis() - _start;
  }

 private:
  uint32_t _start;
};
