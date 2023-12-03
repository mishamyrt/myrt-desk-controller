// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "DomainCommander.h"

bool DomainCommander::handle(uint8_t *message, size_t length, SocketResponse *resp) {
  for (uint8_t i = 0; i < this->_count; i++) {
    if (this->_domains[i] != NULL && this->_domains[i]->code() == message[0]) {
      return this->_domains[i]->handle(&message[1], length - 1, resp);
    }
  }
  return false;
}

bool DomainCommander::add(Domain *domain) {
  if (this->_count >= COMMANDER_DOMAINS_MAX) {
    return false;
  }
  this->_domains[this->_count] = domain;
  this->_count = this->_count + 1;
  domain->initialize();
  return true;
}
