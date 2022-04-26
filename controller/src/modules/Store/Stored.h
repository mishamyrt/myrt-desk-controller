#pragma once

#include <Arduino.h>

class Stored {
  public:
    uint8_t *data;
    uint16_t size;
    uint8_t key;
    uint16_t addr;
    bool shouldUpdate = false;

    template <typename T> Stored(T &data) {
      this->data = (uint8_t*) &data;
      size = sizeof(T);
      key = random(255);
    }

    void update() {
      shouldUpdate = true;
    }
};

