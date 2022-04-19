#pragma once

#include <Arduino.h>

class TimeProgress {
  public:
    void setDuration(unsigned int duration) {
      _duration = duration;
      reset();
    }

    void reset() {
      _start_time = millis();
      _value = 0;
    }

    void tick() {
      _value = constrain(map(
        millis() - _start_time, // Map time delta
        0, _duration,  // from range is 0 - _duration
        0, UINT8_MAX // to uint8_t range (0 - 255)
      ), 0, UINT8_MAX);
    }

    bool is_done() {
      return _value == 255;
    }

    uint8_t value() {
      return _value;
    }

  private:
    unsigned long _start_time = 0;
    unsigned int _duration = 0;
    uint8_t _value = 255;
};
