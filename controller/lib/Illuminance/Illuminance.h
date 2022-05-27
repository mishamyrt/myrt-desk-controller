// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "Arduino.h"

class IlluminanceSensor {
 public:
  IlluminanceSensor(uint8_t sensor_pin, size_t t_resistance) {
    pin = sensor_pin;
    resistance = t_resistance;
  }

  int getValue() {
    return analogRead(pin);
  }

 private:
  uint8_t pin;
  size_t resistance;

  // TODO: Добавить настоящий расчёт с учётом сопротивления резистора
  int normalize(int input)   {
    return map(input, 0, 1000, 0, 2000);
  }
};
