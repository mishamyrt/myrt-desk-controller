// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "HeightController.h"

HeightController::HeightController(Bekant *motor, SonicMotion *motion) {
  _motor = motor;
  _motion = motion;
  _motion->setListener(this);
  _state.height = _motor->getHeight();
  _motion->enable();
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
    _motion->enable();
    return;
  }
  _motion->handle();
}

void HeightController::onUpMotion() {
  _motor->moveUp();
}

void HeightController::onDownMotion() {
  _motor->moveDown();
}

void HeightController::onHoldMotion() {
  _motor->stop();
}

void HeightController::onMotionEnd(size_t distance) {
  _motor->stop();
  _state.height = _motor->getHeight();
}

size_t HeightController::get() {
  return _state.height;
}
