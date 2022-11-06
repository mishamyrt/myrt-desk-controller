// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Response.h"

CommanderResponse::CommanderResponse(AsyncUDPPacket *packet) {
  this->_packet = packet;
}

void CommanderResponse::setCommand(uint8_t command) {
  this->_response[2] = command;
}

void CommanderResponse::setDomain(uint8_t domain) {
  this->_response[1] = domain;
}

void CommanderResponse::flush() {
  this->_response[0] = this->_length + 2;
  this->_packet->write(
    this->_response,
    this->_length + 3
  );
  this->_sent = true;
}

bool CommanderResponse::sent() {
  return this->_sent;
}

bool CommanderResponse::append(uint8_t data) {
  // The zero element stores the length. Clients will expect response in this format
  if (this->_length >= RESPONSE_MAX_LENGTH) {
    return false;
  }
  this->_response[this->_length + 3] = data;
  this->_length = this->_length + 1;
  return true;
}
