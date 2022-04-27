// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>
#include <Timer.h>
#include <Stopwatch.h>
#include "lib/Bekant/Bekant.h"
#include "lib/SonicMotion/SonicMotion.h"

#include <Loggr.h>

#define MIN_HEIGHT 650
#define MAX_HEIGHT 1250

struct HeightState {
  size_t height = 0;
  size_t headroom;
};

class HeightController : public MotionListener {
 public:
  HeightController(Bekant *motor, SonicMotion *motion);

  size_t get();
  bool set(size_t height);
  void handle();

  void onUpMotion() override;
  void onDownMotion() override;
  void onHoldMotion() override;
  void onMotionEnd(size_t distance) override;

 private:
  Bekant *_motor;
  SonicMotion *_motion;
  size_t _target_height;
  HeightState _state;
  Timer _measure_height;
  bool _should_update;
  bool _moving_to_target;

  void _updateHeightLater() {
    _measure_height.set(200);
    _should_update = true;
  }

  void _updateHeight() {
    _updateHeight(_motion->measureDistance() * 10);
  }

  void _updateHeight(size_t distance) {
    _state.height = _state.headroom - distance;
  }
};
