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

struct HeightState {
  size_t height;
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
  Timer _move;
  bool _should_update;
  bool _moving_to_target;
};
