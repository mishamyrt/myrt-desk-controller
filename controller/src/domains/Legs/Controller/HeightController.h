// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>
#include <Timer.h>
#include <Stopwatch.h>
#include <Loggr.h>
#include "../Bekant/Bekant.h"

struct HeightState {
  size_t height;
  size_t headroom;
};

class HeightController {
 public:
  HeightController(Bekant *motor);

  size_t get();
  bool set(size_t height);
  void handle();

 private:
  Bekant *_motor;
  size_t _target_height;
  HeightState _state;
  Timer _move;
  bool _should_update;
  bool _moving_to_target;
};
