// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Legs.h"
#include "constants.h"

Bekant Motors(PIN_BEKANT_UP, PIN_BEKANT_DOWN);
HeightController Height(&Motors);

Domain LegsDomain(DOMAIN_LEGS, [](Domain *domain) {
  domain->on(COMMAND_LEGS_READ, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l > 0) {
      return false;
    }
    size_t height = Height.get();
    r->append(highByte(height));
    r->append(lowByte(height));
    r->flush();
    return true;
  });
});
