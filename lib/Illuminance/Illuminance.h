#pragma once

#include "Arduino.h"

class IlluminanceSensor {
  public:
    IlluminanceSensor(uint8_t sensor_pin, size_t t_resistance) {
      pin = sensor_pin;
      resistance = t_resistance;
    }

    int getValue() {
      return normalize(
        analogRead(pin)
      );
    }

  private:
    uint8_t pin;
    size_t resistance;

    // TODO: Добавить настоящий расчёт с учётом сопротивления резистора
    int normalize(int input)   {
      return map(input, 0, 1000, 0, 2000);
    }
};
