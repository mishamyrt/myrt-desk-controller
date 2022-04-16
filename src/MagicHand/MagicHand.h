#pragma once

#include "NewPing.h"
#include "Bekant.h"
#include "pins.h"
#include "Loggr.h"

#define MH_INTERVAL 60
#define MH_ACTION_DISTANCE 4

enum {
  kStateNone,
  kStateFixed,
  kStateUp,
  kStateDown,
} MagicHandState;

class MagicHand {
  public:
    MagicHand(BekantHeight *desk) {
      _desk = desk;
    }

    void handle() {
      if (millis() < _next_ping) {
        return;
      }
      _next_ping = millis() + MH_INTERVAL;
      int distance = _measureDistance();
      if (distance > 45) {
        if (moveToState(kStateNone)) {
          _desk->stop();
        }
        return;
      }
      switch (_state) {
        case kStateNone:
          if (moveToState(kStateFixed)) {
            _action_start_distance = distance;
          }
          break;
        case kStateFixed:
          if (distance - _action_start_distance >= MH_ACTION_DISTANCE) {
            moveToState(kStateUp);
          } else if (_action_start_distance - distance >= MH_ACTION_DISTANCE) {
            moveToState(kStateDown);
          } else {
            moveToState(kStateFixed);
            _desk->stop();
          }
          break;
        case kStateUp:
          if (distance < _action_start_distance) {
            moveToState(kStateFixed);
            return;
          }
          moveToState(kStateUp);
          _desk->increase();
          break;
        case kStateDown:
          if (distance > _action_start_distance) {
            moveToState(kStateFixed);
            return;
          }
          moveToState(kStateDown);
          _desk->decrease();
          break;
      }
    }

    bool moveToState(uint8_t state) {
      if (state != _target_state) {
        _target_state = state;
        _state_count = 0;
        return false;
      }
      if (state == _state) {
        return false;
      }
      _state_count++;
      if (_state_count == 3) {
        _state_count = 0;
        _state = _target_state;
        Loggr.message = "Changing state to";
        Loggr.message += _state;
        Loggr.flush();
        return true;
      }
      return false;
    }
    //   if (distance >)


    //     // unsigned int uS = sonar.ping();
    //     // int length = uS / US_ROUNDTRIP_CM;
    //     // if (length <= 7) {
    //     //   Height.decrease();
    //     // } else if (length >= 16 && length < 45) {
    //     //   Height.increase();
    //     // } else {
    //     //   loops_without_actions++;
    //     // }
    //     // if (loops_without_actions == 5) {
    //     //   loops_without_actions = 0;
    //     //   Height.stop();
    //     // }
    //     // Loggr.print(String(length));
    //     // next_sonic_tick = millis() + 100;
    // }

  private:
    uint8_t _state = kStateNone;
    uint8_t _state_count;
    uint8_t _target_state;
    BekantHeight *_desk;
    NewPing _sonar = NewPing(PIN_USONIC_TRIGGER, PIN_USONIC_ECHO, 250);
    unsigned long _next_ping = 0;
    int _action_start_distance;

    int _measureDistance() {
      unsigned int uS = _sonar.ping();
      return uS / US_ROUNDTRIP_CM;
    }
};
