// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Timer.h>
// #include <VL53L0X.h>
#include "Listener.h"
#include "lib/Proven.h"

#define SONIC_PING_INTERVAL 50
#define MOTION_ACTION_PROOFS 4
// Trigger zone
#define MOTION_MAX_HEIGHT 500
#define MOTION_DEAD_ZONE_DOWN 50
#define MOTION_DEAD_ZONE_UP 50

enum {
  STATE_FREE,
  STATE_HOLD,
  STATE_UP,
  STATE_DOWN
};

class SonicMotion {
 public:
  SonicMotion();
  void handle();
  bool disable();
  void enable();
  void setListener(MotionListener *listener);

 private:
  bool _enabled = false;
  size_t _distance;
  size_t _motion_start_distance;
  size_t _dead_zone_top;
  size_t _dead_zone_bottom;
  Proven<uint8_t> _state = Proven<uint8_t>(STATE_FREE);
  MotionListener *_listener;
  // VL53L0X sensor;
  Timer _ping_debounce;

  bool _update();
  uint8_t _assumeState();
};
