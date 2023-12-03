#include "System.h"
#include "constants.h"
#include <Arduino.h>

Domain SystemDomain(DOMAIN_SYSTEM, [](Domain *domain) {
  domain->on(COMMAND_SYSTEM_REBOOT, [](uint8_t *m, size_t l, SocketResponse *r) {
    ESP.restart();
    return true;
  });
  domain->on(COMMAND_SYSTEM_PING, [](uint8_t *m, size_t l, SocketResponse *r) {
    return true;
  });
  domain->on(COMMAND_SYSTEM_FREE_HEAP, [](uint8_t *m, size_t l, SocketResponse *r) {
    uint32_t heap = ESP.getFreeHeap();
    r->append(highByte(heap));
    r->append(lowByte(heap));
    r->broadcast();
    return true;
  });
});

