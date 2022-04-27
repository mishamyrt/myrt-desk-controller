// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "CommandServer.h"

void CommandServer::begin(Stream *serial) {
  _data.setHandler(this);
  _data.connect(serial);
}

CommandServer& CommandServer::on(
  uint8_t code,
  bool (*handler)(uint8_t *message, uint8_t length)
) {
  _handlers[_handlers_count].code = code;
  _handlers[_handlers_count].handle = handler;
  _handlers_count++;
  return *this;
}

void CommandServer::handle() {
  _data.handle();
}

bool CommandServer::handleMessage(uint8_t *message, uint8_t length) {
  for (uint8_t i = 0; i < _handlers_count; i++) {
    if (_handlers[i].code == message[0]) {
      return _handlers[i].handle(message, length);
    }
  }
  return false;
}
