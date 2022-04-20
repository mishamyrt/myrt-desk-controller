#pragma once

#include <Arduino.h>

class Timer {
  public:
    bool isDone() {
      return millis() >= _end;
    }

    void set(size_t ms) {
      _end = millis() + ms;
    }

  private:
    unsigned long _end = 0;
};
