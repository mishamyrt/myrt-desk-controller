// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "DomainCommander.h"

void DomainCommander::handle(AsyncUDPPacket *packet) {
  uint8_t *message = packet->data();
  size_t length = packet->length();
  if (length < 3 || length - 1 != message[0]) {
    return;
  }
  // Find domain
  bool success = false;
  CommanderResponse *resp = new CommanderResponse(packet);
  for (uint8_t i = 0; i < this->_head; i++) {
    if (this->_domains[i].code == message[1]) {
      // Handle command on domain
      resp->setDomain(message[1]);
      success = this->_domains[i].handle(&message[2], length - 2, resp);
      break;
    }
  }
  if (resp->sent()) {
    return;
  }
  resp->append(message[1]);
  resp->append(message[2]);
  if (success) {
    resp->append(0);
  } else {
    resp->append(1);
  }
  resp->flush();
  delete resp;
}

bool DomainCommander::add(Code code, DomainInitializer init) {
  if (this->_head >= COMMANDER_DOMAINS_MAX) {
    return false;
  }
  this->_domains[this->_head].code = code;
  init(&this->_domains[this->_head]);
  this->_head++;
  return true;
}
