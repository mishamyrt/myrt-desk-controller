// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "SonicMotion.h"
#include <Loggr.h>
#include <Blink.h>
#include <Wire.h>

SonicMotion::SonicMotion() {
  // Wire.begin();
  // sensor.setTimeout(500);
  // if (!sensor.init()) {
  //   Loggr.print("Failed to detect and initialize TOF sensor!");
  //   blink(10);
  //   return;
  // }
}

void SonicMotion::setListener(MotionListener *listener) {
  _listener = listener;
}

void SonicMotion::handle() {
  if (!_enabled || !_update()) {
    return;
  }
  switch (_assumeState()) {
    case STATE_FREE:
      if (_state.set(STATE_FREE, 2)) {
        _listener->onMotionEnd(_distance);
      }
      break;
    case STATE_HOLD:
      if (_state.set(STATE_HOLD, 4)) {
        // Save values for next calculations
        _motion_start_distance = _distance;
        _dead_zone_top = _distance + MOTION_DEAD_ZONE_UP;
        if (_distance < MOTION_DEAD_ZONE_DOWN) {
          _dead_zone_bottom = 0;
        } else {
          _dead_zone_bottom = _distance - MOTION_DEAD_ZONE_DOWN;
        }
        // Call listener
        _listener->onHoldMotion();
      }
      break;
    case STATE_UP:
      if (_state.set(STATE_UP, 2)) {
        _listener->onUpMotion();
      }
      break;
    case STATE_DOWN:
      if (_state.set(STATE_DOWN, 2)) {
        _listener->onDownMotion();
      }
      break;
  }
}

uint8_t SonicMotion::_assumeState() {
  uint8_t state = _state.get();
  if (state == STATE_FREE) {
    if (_distance > MOTION_MAX_HEIGHT || _distance < MOTION_DEAD_ZONE_DOWN) {
      return STATE_FREE;
    } else {
      return STATE_HOLD;
    }
  } else if (state == STATE_HOLD) {
    if (_distance <= _dead_zone_top && _distance >= _dead_zone_bottom) {
      return STATE_HOLD;
    }
  }
  if ((state == STATE_UP
      && (_distance <= _motion_start_distance
        && _distance > _dead_zone_bottom))
    ||(state == STATE_DOWN
      && (_distance >= _motion_start_distance
        && _distance > _dead_zone_top))) {
    return STATE_HOLD;
  }
  if (_distance > _motion_start_distance + MOTION_MAX_HEIGHT
    || _distance == 0) {
    return STATE_FREE;
  }

  if (_distance < _dead_zone_bottom) {
    return STATE_DOWN;
  } else if (_distance > _dead_zone_top) {
    return STATE_UP;
  }
  // Execution should not reach here.
  return STATE_FREE;
}

bool SonicMotion::_update() {
  if (!_ping_debounce.isDone()) {
    return false;
  }
  _ping_debounce.set(SONIC_PING_INTERVAL);
  // _distance = sensor.readRangeSingleMillimeters();
  // if (sensor.timeoutOccurred()){
  //   return false;
  // }
  return true;
}

bool SonicMotion::disable() {
  if (_state.get() != STATE_FREE) {
    return false;
  }
  _enabled = false;
  return true;
}

void SonicMotion::enable() {
  _enabled = true;
}

