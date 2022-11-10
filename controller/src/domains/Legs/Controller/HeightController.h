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
#include "../ToF/SensorReader.h"

#define STOPPING_DISTANCE_UP 20
#define STOPPING_DISTANCE_DOWN 10

struct HeightState {
  size_t height;
};

class HeightController {
 public:
  HeightController(Bekant *motor, SensorReader *reader);

  bool initialize();
  size_t get();
  bool set(size_t height);
  void handle();

 private:
  Bekant *_motor;
  SensorReader *_reader;
  size_t _target_height;
  uint16_t _current_distance = 0;
  HeightState _state;
  bool _moving_to_target;
};
