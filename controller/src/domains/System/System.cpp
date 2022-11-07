#include "System.h"
#include "constants.h"

Domain SystemDomain(DOMAIN_SYSTEM, [](Domain *domain) {
  domain->on(COMMAND_SYSTEM_REBOOT, [](uint8_t *m, size_t l, CommanderResponse *r) {
    ESP.restart();
    return true;
  });
  domain->on(COMMAND_SYSTEM_PING, [](uint8_t *m, size_t l, CommanderResponse *r) {
    return true;
  });
});

