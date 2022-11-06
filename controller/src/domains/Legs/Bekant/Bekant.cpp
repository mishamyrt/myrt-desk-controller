// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Bekant.h"

Bekant::Bekant(uint8_t pin_up, uint8_t pin_down) {
  pinMode(pin_up, OUTPUT);
  pinMode(pin_down, OUTPUT);
  _pin_up = pin_up;
  _pin_down = pin_down;
}

void Bekant::stop() {
  digitalWrite(_pin_down, LOW);
  digitalWrite(_pin_up, LOW);
  uint16_t move_time = millis() - _start_time;
  uint16_t height_diff = move_time - BEKANT_MOTOR_START;
  if (_state == STATE_READY) {
    return;
  } else if (move_time < BEKANT_MOTOR_START) {
    _state = STATE_READY;
    return;
  }
  switch (_state) {
    case STATE_MOVE_UP:
      height_diff *= BEKANT_SPEED_UP;
      _height = min(_height + height_diff, BEKANT_MAX_HEIGHT);
      break;
    case STATE_MOVE_DOWN:
      height_diff *= BEKANT_SPEED_DOWN;
      _height = max(_height - height_diff, BEKANT_MIN_HEIGHT);
      break;
  }
}

uint32_t Bekant::getMoveTime(uint16_t target_height) {
  if (target_height == _height) {
    return 0;
  } else if (target_height > _height) {
    return (target_height - _height) / BEKANT_SPEED_UP;
  } else {
    return (_height - target_height) / BEKANT_SPEED_DOWN;
  }
}

void Bekant::moveDown() {
  digitalWrite(_pin_up, LOW);
  digitalWrite(_pin_down, HIGH);
  _state = STATE_MOVE_DOWN;
  _start_time = millis();
}

void Bekant::moveUp() {
  digitalWrite(_pin_down, LOW);
  digitalWrite(_pin_up, HIGH);
  _state = STATE_MOVE_UP;
  _start_time = millis();
}

uint16_t Bekant::getHeight() {
  return _height;
}

uint8_t Bekant::getState() {
  return _state;
}
