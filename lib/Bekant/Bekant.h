#pragma once

#include "Arduino.h"
#include "states.h"

#define SPEED 0.033519553
#define DELAY 500
#define MAX_TIME 20000
#define MIN_HEIGHT 650
#define MAX_HEIGHT 1250

class BekantHeight {
  public:
    void initialize(uint8_t up, uint8_t down) {
      pin_up = up;
      pin_down = down;
      pinMode(pin_up, OUTPUT);
      pinMode(pin_down, OUTPUT);
      calibrate();
    }

    bool isReady() {
      return state == STATE_READY;
    }

    uint16_t get() {
      return height;
    }

    void handle() {
      if (isReady()) return;
      switch (state) {
        case STATE_CALIBRATION:
          handleCalibration();
          break;
        case STATE_MOVE_TO_TARGET:
          handleTarget();
          break;
        }
    }

    void increase() {
      if (height >= MAX_HEIGHT) {
        return;
      }
      switch (state) {
        case STATE_READY:
          action_start = millis() + DELAY;
          initial_height = height;
          state = STATE_MOVE_UP;
          writeUp();
          break;
        case STATE_MOVE_UP:
          uint16_t diff = (millis() - action_start) * SPEED;
          if (diff > 0 && diff < MAX_HEIGHT) {
            height = initial_height + diff;
          }
          break;
      }
    }

    void decrease() {
      if (height <= MIN_HEIGHT) {
        return;
      }
      switch (state) {
        case STATE_READY:
          action_start = millis() + DELAY;
          initial_height = height;
          state = STATE_MOVE_DOWN;
          writeDown();
          break;
        case STATE_MOVE_DOWN:
          uint16_t diff = (millis() - action_start) * SPEED;
          if (diff > 0 && diff < MAX_HEIGHT) {
            height = initial_height - diff;
          }
          break;
      }
    }

    void stop() {
      digitalWrite(pin_down, LOW);
      digitalWrite(pin_up, LOW);
      state = STATE_READY;
    }

    void calibrate() {
      action_start = millis();
      state = STATE_CALIBRATION;
      writeDown();
    }

    void set(uint16_t target) {
      action_start = millis() + DELAY;
      target_height = target;
      initial_height = height;
      state = STATE_MOVE_TO_TARGET;
      if (target > height) {
        writeUp();
      } else {
        writeDown();
      }
    }

  private:
    uint8_t state;
    uint16_t initial_height;
    uint16_t height;
    uint16_t target_height;
    uint32_t action_start;
    uint32_t action_end;
    uint32_t now;

    uint8_t pin_up;
    uint8_t pin_down;

    void writeDown() {
      digitalWrite(pin_down, HIGH);
    }

    void writeUp() {
      digitalWrite(pin_up, HIGH);
    }

    void handleTarget() {
      now = millis();
      uint16_t diff = (now - action_start) * SPEED;
      if (diff < MAX_HEIGHT) {
        if (target_height > initial_height) {
          height = initial_height + diff;
        } else {
          height = initial_height - diff;
        }
      }
      if (height == target_height) {
        stop();
      }
    }

    void handleCalibration() {
      if (millis() - action_start < MAX_TIME) return;
      stop();
      height = MIN_HEIGHT;
      initial_height = height;
      state = STATE_READY;
    }
};
