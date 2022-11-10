// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include <Adafruit_VL53L0X.h>

class SensorReader {
  public:
    bool connect();
    bool connected();

    uint16_t get_value(uint8_t resolution = 1);

  private:
    bool _connected = false;
    uint8_t _i = 0;
    ulong _summ = 0;
    Adafruit_VL53L0X _sensor = Adafruit_VL53L0X();
    VL53L0X_RangingMeasurementData_t _measure;
};
