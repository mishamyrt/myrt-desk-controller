#pragma once

#include "stdint.h"

class DapHandler {
  public:
    virtual bool handleMessage(uint8_t *message, uint8_t message_length) = 0;
};
