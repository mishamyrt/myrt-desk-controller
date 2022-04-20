#pragma once

#include <Arduino.h>

enum {
  STATE_READY,
  STATE_MOVE_UP,
  STATE_MOVE_DOWN
};

class Bekant {
  public:
    Bekant(uint8_t up, uint8_t down);

    void stop();
    void moveDown();
    void moveUp();
    uint8_t getState();

  private:
    uint8_t _pin_up;
    uint8_t _pin_down;
    uint8_t _state = STATE_READY;
};
