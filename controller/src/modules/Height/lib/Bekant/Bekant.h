// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>

#define BEKANT_SPEED_UP 0.03178807947
#define BEKANT_SPEED_DOWN 0.03577604198
#define BEKANT_MOTOR_START 250
#define BEKANT_MAX_HEIGHT 1250
#define BEKANT_MIN_HEIGHT 650

enum {
  STATE_READY,
  STATE_MOVE_UP,
  STATE_MOVE_DOWN
};

class Bekant {
 public:
  Bekant(uint8_t up, uint8_t down);

  void stop();
  void moveDown();
  void moveUp();
  uint16_t getHeight();
  uint32_t getMoveTime(uint16_t target_height);
  uint8_t getState();

 private:
  uint8_t _pin_up;
  uint8_t _pin_down;
  uint8_t _state = STATE_READY;
  uint16_t _height = BEKANT_MIN_HEIGHT;
  uint64_t _start_time;
};
