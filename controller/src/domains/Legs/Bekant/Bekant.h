// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>

enum {
  STATE_READY,
  STATE_MOVE_UP,
  STATE_MOVE_DOWN
};

#define BEKANT_MAX_HEIGHT 1250
#define BEKANT_MIN_HEIGHT 650

class Bekant {
 public:
  Bekant(uint8_t up, uint8_t down);

  void stop();
  void moveDown();
  void moveUp();
  uint8_t getState();

 private:
  uint8_t _pin_up;
  uint8_t _pin_down;
  uint8_t _state = STATE_READY;
};
