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
  _state = STATE_READY;
}

void Bekant::moveDown() {
  digitalWrite(_pin_up, LOW);
  digitalWrite(_pin_down, HIGH);
  _state = STATE_MOVE_DOWN;
}

void Bekant::moveUp() {
  digitalWrite(_pin_down, LOW);
  digitalWrite(_pin_up, HIGH);
  _state = STATE_MOVE_UP;
}

uint8_t Bekant::getState() {
  return _state;
}
