// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "HeightController.h"

HeightController::HeightController(Bekant *motor) {
  _motor = motor;
  _state.height = _motor->getHeight();
}

bool HeightController::set(size_t height) {
  if (height == _state.height) {
    return true;
  }
  Loggr.message = "Move time ";
  Loggr.message += _motor->getMoveTime(height);
  Loggr.flush();
  _move.set(_motor->getMoveTime(height));
  if (height < _state.height) {
    _motor->moveDown();
  } else if (height > _state.height) {
    _motor->moveUp();
  }
  _target_height = height;
  _moving_to_target = true;
  return true;
}

void HeightController::handle() {
  if (_moving_to_target && _move.isDone()) {
    _motor->stop();
    _state.height = _motor->getHeight();
    _moving_to_target = false;
    return;
  }
}

size_t HeightController::get() {
  return _state.height;
}
