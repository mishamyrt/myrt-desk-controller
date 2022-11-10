// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Response.h"

CommanderResponse::CommanderResponse(AsyncUDPPacket *packet) {
  _packet = packet;
}

void CommanderResponse::setCommand(uint8_t command) {
  _response[2] = command;
}

void CommanderResponse::setDomain(uint8_t domain) {
  _response[1] = domain;
}

AsyncUDPPacket *CommanderResponse::packet() {
  return _packet;
}

void CommanderResponse::flush() {
  _response[0] = _length + 2;
  _packet->write(
    _response,
    _length + 3
  );
  _sent = true;
}

bool CommanderResponse::sent() {
  return _sent;
}

bool CommanderResponse::append(uint8_t data) {
  // The zero element stores the length. Clients will expect response in this format
  if (_length >= RESPONSE_MAX_LENGTH) {
    return false;
  }
  _response[_length + 3] = data;
  _length = _length + 1;
  return true;
}
