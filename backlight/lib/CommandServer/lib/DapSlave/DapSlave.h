// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "Stream.h"
#include "DapHandler.h"

#define DAP_INPUT_BUFFER_SIZE 128

enum {
  HEADER_FIRST = 0xFE,
  HEADER_SECOND = 0xEF
};

enum {
  STATUS_SUCCESS = 0xEE,
  STATUS_ERROR = 0xFF
};

class DapSlave {
 public:
  void setHandler(DapHandler *handler) {
    _handler = handler;
  }

  void connect(Stream *data);
  void handle();

 private:
  // I/O
  Stream *_serial;
  DapHandler *_handler = NULL;
  // Message buffer
  uint8_t _input_buffer[DAP_INPUT_BUFFER_SIZE];
  uint8_t _input_length;
  // Internal variables for input values
  uint8_t _current_value;
  uint8_t _previous_value;

  void _sendError();
  void _sendSuccess();
  void _sendCode(uint8_t code);

  bool _readMessage();
  bool _readHeader();
  bool _readBody();
  bool _findStartSequence();
};
