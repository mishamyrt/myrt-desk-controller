#pragma once

#include <stdint.h>

class Flagpole {
  public:
    void set(uint8_t flags) {
      _data |= flags;
    }

    void unset(uint8_t flags) {
      _data &= ~flags;
    }

    bool isSet(uint8_t flags) {
      return _data & flags;
    }

    bool isEmpty() {
      return _data == 0;
    }

  private:
    uint8_t _data = 0;
};
