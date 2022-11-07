// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "DomainCommander.h"

void DomainCommander::handle(AsyncUDPPacket *packet) {
  uint8_t *message = packet->data();
  size_t length = packet->length();
  size_t payload_length = 0;
  if (length < 3) {
    return;
  } else if (message[0] == 111 && length > 3) {
    payload_length = (message[1] << 8) + message[2];
  } else {
    payload_length = message[0];
  }
  if (payload_length != length - 1) {
    return;
  }

  // Find domain
  bool success = false;
  CommanderResponse *resp = new CommanderResponse(packet);
  for (uint8_t i = 0; i < this->_count; i++) {
    if (this->_domains[i] != NULL && this->_domains[i]->code() == message[1]) {
      // Handle command on domain
      resp->setDomain(message[1]);
      success = this->_domains[i]->handle(&message[2], length - 2, resp);
      break;
    }
  }
  if (resp->sent()) {
    delete resp;
    return;
  }
  if (success) {
    resp->append(0);
  } else {
    resp->append(1);
  }
  resp->flush();
  delete resp;
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
