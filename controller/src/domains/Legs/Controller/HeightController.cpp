// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "HeightController.h"
#include <Loggr.h>
#include <config.h>

HeightController::HeightController(Bekant *motor, SensorReader *reader) {
  _motor = motor;
  _reader = reader;
}

bool HeightController::initialize() {
  if (_reader != NULL && _reader->connected()) {
    Store.append(&_descriptor);
    _reader->setCorrection(_correction);
    _height = _reader->getValue(20);
    return true;
  }
  return false;
}

bool HeightController::set(size_t height) {
  if (height < BEKANT_MIN_HEIGHT || height > BEKANT_MAX_HEIGHT) {
    return false;
  }
  size_t mapped_height = height - BEKANT_MIN_HEIGHT;
  if (mapped_height == _height) {
    return true;
  } else if (mapped_height < _height) {
    _motor->moveDown();
  } else if (mapped_height > _height) {
    _motor->moveUp();
  }
  _target_height = mapped_height;
  return true;
}

bool HeightController::calibrate() {
  _state = _motor->getState();
  if (_state != STATE_READY) {
    return false;
  }
  _calibrating = true;
  return true;
}

void HeightController::_calibrate() {
  _motor->stop();
  _motor->moveDown();
  uint16_t _previous_distance = BEKANT_MAX_HEIGHT;
  _reader->setCorrection(0);
  while (_calibrating) {
    _current_distance = _reader->getValue(20);
    if (_previous_distance - _current_distance < 5) {
      _motor->stop();
      _current_distance = _reader->getValue(32);
      _descriptor.update();
      _reader->setCorrection(_current_distance);
      _height = 0;
      _calibrating = false;
      return;
    } else {
      _previous_distance = _current_distance;
    }
  }
}

bool HeightController::handle() {
  if (_reader == NULL || !_reader->connected()) {
    return false;
  }
  _state = _motor->getState();
  if (_calibrating) {
    _calibrate();
  } else if (_state != STATE_READY) {
    _current_distance = _reader->getValue(8);
    if (
      (_state == STATE_MOVE_UP && _current_distance + STOPPING_DISTANCE_UP >= _target_height) ||
      (_state == STATE_MOVE_DOWN && _current_distance - STOPPING_DISTANCE_DOWN <= _target_height)
    ) {
      _motor->stop();
      _height = _reader->getValue(20);
    }
  }
  return true;
}

size_t HeightController::get() {
  return _height + BEKANT_MIN_HEIGHT;
}
