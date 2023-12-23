// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Legs.h"
#include "constants.h"
#include "ToF/SensorReader.h"

Bekant Motors(PIN_BEKANT_UP, PIN_BEKANT_DOWN);
SensorReader HeightReader = SensorReader();
HeightController Height(&Motors, &HeightReader);

uint8_t legsState(SocketResponse *r) {
  size_t height = Height.get();
  r->append(highByte(height));
  r->append(lowByte(height));
  return COMMAND_LEGS_BROADCAST_STATE;
}

Domain LegsDomain(DOMAIN_LEGS, [](Domain *domain) {
  HeightReader.connect();
  Height.initialize();
  domain->on(COMMAND_LEGS_READ, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l > 0) {
      return false;
    }
    r->reply(legsState);
    return true;
  });
  domain->on(COMMAND_LEGS_SET_HEIGHT, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l != 2) {
      return false;
    }
    size_t height = (m[0] << 8) + m[1];
    bool success = Height.set(height);
    return success;
  });
  domain->on(COMMAND_LEGS_CALIBRATE, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l != 0) {
      return false;
    }
    return Height.calibrate();
  });
});
