#pragma once
#include <Arduino.h>
#include "SocketResponse.h"

class SocketMessageHandler {
  public:
    virtual bool handle(uint8_t *message, size_t length, SocketResponse *resp) = 0;
};
