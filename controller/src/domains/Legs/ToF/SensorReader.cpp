// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "SensorReader.h"

bool SensorReader::connect() {
  if (!_connected && !_sensor.begin()) {
    return false;
  }
  return true;
}

bool SensorReader::connected() {
  return _connected;
}

uint16_t SensorReader::get_value(uint8_t resolution) {
  _summ = 0;
  for (_i = 0; _i < resolution; _i++) {
    _sensor.rangingTest(&_measure, false);
    if (_measure.RangeStatus == 4) {
      return 0;
    }
    _summ += _measure.RangeMilliMeter;
  }
  if (resolution == 1) {
    return _summ;
  }
  return uint16_t(_summ / resolution);
}
