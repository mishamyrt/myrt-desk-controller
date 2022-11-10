// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>

class Stored {
 public:
  uint8_t *data;
  uint16_t size;
  uint8_t key;
  uint16_t addr = 0;
  bool shouldUpdate = false;

  template <typename T> Stored(const T &data, uint8_t key) {
    this->data = (uint8_t*) &data;  // NOLINT(readability/casting)
    size = sizeof(T);
    this->key = key;
  }

  void update() {
    shouldUpdate = true;
  }
};

