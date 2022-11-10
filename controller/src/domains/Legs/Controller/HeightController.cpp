// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "HeightController.h"

HeightController::HeightController(Bekant *motor, SensorReader *reader) {
  _motor = motor;
  _reader = reader;
  _state.height = _motor->getHeight();
}

bool HeightController::initialize() {
  if (_reader != NULL && _reader->connected()) {
    _state.height = _reader->getValue(20);
    return true;
  }
  return false;
}

bool HeightController::set(size_t height) {
  if (height == _state.height) {
    return true;
  }
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
  if (_reader == NULL || !_reader->connected()) {
    return;
  }
  if (_moving_to_target) {
    _current_distance = _reader->getValue(5);
    if (
      (_motor->getState() == STATE_MOVE_UP && _current_distance + STOPPING_DISTANCE_UP >= _target_height) ||
      (_motor->getState() == STATE_MOVE_DOWN && _current_distance - STOPPING_DISTANCE_DOWN <= _target_height)
    ) {
      _motor->stop();
      _state.height = _reader->getValue(20);
      _moving_to_target = false;
    }
    return;
  }
}

size_t HeightController::get() {
  return _state.height;
}
