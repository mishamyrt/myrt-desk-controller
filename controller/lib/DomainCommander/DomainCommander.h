// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "AsyncUDP.h"
#include "Domain.h"

#define COMMANDER_DOMAINS_MAX 4

typedef std::function<void(Domain *domain)> DomainInitializer;

class DomainCommander {
  public:
    DomainCommander() {

    }
    void handle(AsyncUDPPacket *packet);

    bool add(Domain *domain);

  private:
    AsyncUDP *_connection;
    Domain *_domains[COMMANDER_DOMAINS_MAX];
    uint8_t _count = 0;
};

