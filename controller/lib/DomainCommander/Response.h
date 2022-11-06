// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.
#pragma once

#include "AsyncUDP.h"

#define RESPONSE_MAX_LENGTH 16

class CommanderResponse {
  public:
    CommanderResponse(AsyncUDPPacket *packet);

    void flush();

    void setCommand(uint8_t command);

    void setDomain(uint8_t domain);

    bool sent();

    bool append(uint8_t data);

  private:
    uint8_t _response[RESPONSE_MAX_LENGTH + 3];
    uint8_t _length = 0;
    AsyncUDPPacket *_packet;
    bool _sent = false;
};
