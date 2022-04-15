#pragma once

#include "stdint.h"

enum {
  D_CREATED,
  D_SENT,
  D_RECIEVED,
};

struct DapRequest {
  uint8_t state = D_CREATED;
  uint8_t *payload;
  uint8_t payload_length;
  unsigned long expiry = 0;
};
