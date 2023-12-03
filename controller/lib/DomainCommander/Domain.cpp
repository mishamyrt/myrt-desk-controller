#include "Domain.h"

bool Domain::on(uint8_t code, AsyncCommandHandler handle) {
  if (this->_head >= DOMAIN_MAX_COMANDS) {
    return false;
  }
  this->_commands[this->_head].code = code;
  this->_commands[this->_head].handle = handle;
  this->_head++;
  return true;
}

bool Domain::handle(uint8_t *message, size_t length, SocketResponse *resp) {
  for (uint8_t i = 0; i < _head; i++) {
    if (_commands[i].code == message[0]) {
      return _commands[i].handle(&message[1], length - 1, resp);
    }
  }
  return false;
}
