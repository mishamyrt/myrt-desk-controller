#pragma once

#include <NewPing.h>
#include <Timer.h>
#include "Listener.h"
#include "lib/Proven.h"

#define SONIC_PING_INTERVAL 30
#define MOTION_ACTION_PROOFS 4
// Trigger zone
#define MOTION_MAX_HEIGHT 50
#define MOTION_DEAD_ZONE_DOWN 14
#define MOTION_DEAD_ZONE_UP 8

enum {
  STATE_FREE,
  STATE_HOLD,
  STATE_UP,
  STATE_DOWN
};

class SonicMotion {
  public:
    SonicMotion(uint8_t trigger, uint8_t echo);
    void handle();
    bool disable();
    void enable();
    void setListener(MotionListener *listener);
    size_t measureDistance();

  private:
    bool _enabled = false;
    size_t _distance;
    size_t _motion_start_distance;
    size_t _dead_zone_top;
    size_t _dead_zone_bottom;
    Proven<uint8_t> _state = Proven<uint8_t>(STATE_FREE);
    MotionListener *_listener;
    NewPing *_sonar;
    Timer _ping_debounce;

    bool _update();
    uint8_t _assumeState();
};
