// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>
#include <Timer.h>
#include <Stopwatch.h>
#include <Loggr.h>
#include <Store.h>
#include "../Bekant/Bekant.h"
#include "../ToF/SensorReader.h"
#include "../constants.h"

#define STOPPING_DISTANCE_UP 18
#define STOPPING_DISTANCE_DOWN 10

class HeightController {
 public:
  HeightController(Bekant *motor, SensorReader *reader);

  bool initialize();
  bool handle();
  bool calibrate();
  bool set(size_t height);
  size_t get();


 private:
  Bekant *_motor;
  SensorReader *_reader;
  size_t _height = 0;
  size_t _target_height = 0;
  uint16_t _current_distance = 0;
  uint16_t _correction = BEKANT_MIN_HEIGHT;
  uint8_t _state = STATE_READY;
  bool _calibrating = false;
  Stored _descriptor = Stored(_correction, DOMAIN_LEGS);

  void _calibrate();
};
