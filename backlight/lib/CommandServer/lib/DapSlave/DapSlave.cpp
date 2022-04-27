// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "DapSlave.h"
#include <Arduino.h>

void DapSlave::connect(Stream *data) {
  _serial = data;
  _sendSuccess();
}

void DapSlave::handle() {
  if (_serial->available() < 4) {
    return;
  }
  delay(10);
  if (!_readMessage()) {
    _sendError();
    return;
  }
  if (_handler != NULL
    && _handler->handleMessage(_input_buffer, _input_length)) {
    _sendSuccess();
  } else {
    _sendError();
  }
}

void DapSlave::_sendError() {
  _sendCode(STATUS_ERROR);
}

void DapSlave::_sendSuccess() {
  _sendCode(STATUS_SUCCESS);
}

void DapSlave::_sendCode(uint8_t code) {
  _serial->write(HEADER_FIRST);  // Header
  _serial->write(HEADER_SECOND);
  _serial->write(1);  // Length
  _serial->write(code);  // Payload
}

bool DapSlave::_readMessage() {
  if (!_readHeader()) {
    return false;
  }
  if (!_readBody()) {
    return false;
  }
  return true;
}

bool DapSlave::_readHeader() {
  if (!_findStartSequence()) {
    return false;
  }
  _input_length = _serial->read();
  if (_input_length > DAP_INPUT_BUFFER_SIZE || _input_length <= 0) {
    return false;
  }
  return true;
}

bool DapSlave::_readBody() {
  uint8_t index = 0;
  while (index < _input_length) {
    _current_value = _serial->read();
    if (_current_value == -1) {
      return false;
    }
    _input_buffer[index] = _current_value;
    index++;
  }
  return true;
}

bool DapSlave::_findStartSequence() {
  while(_serial->available() > 0) {
    _current_value = _serial->read();
    if (_current_value == HEADER_SECOND && _previous_value == HEADER_FIRST) {
      return true;
    }
    _previous_value = _current_value;
  }
  return false;
}
