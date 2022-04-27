// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

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
  uint64_t expiry = millis();
};
