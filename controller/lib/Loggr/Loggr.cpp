// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Loggr.h"
#include <config.h>

void LoggrProvider::setProtocol(AsyncUDP *udp) {
  _udp = udp;
}

#ifdef CONFIG_LOGGING
void LoggrProvider::setClient(AsyncUDPPacket *client) {
  _remote_ip = client->remoteIP();
  _remote_port = client->remotePort();
  if (_udp != NULL) {
    _connected = true;
  }
  if (_message.length() > 0) {
    _send_message();
    _message = "";
  }
}

void LoggrProvider::print(String message) {
   if (_connected) {
    _message += message + "\n";
    _send_message();
    _message = "";
  } else if (LOGGR_PRESERVE_LOGS) {
    _message += message + "\n";
  }
}

void LoggrProvider::_flush() {
  _result = _udp->writeTo(&_message_buffer[0], _message_buffer_length, _remote_ip, _remote_port);
  if (_result == 0) {
    _connected = false;
  }
}
#else
void LoggrProvider::setClient(AsyncUDPPacket *client) {}
void LoggrProvider::print(String message) {}
void LoggrProvider::_flush() {}
#endif



void LoggrProvider::_fillBuffer(const unsigned int length, const unsigned int offset) {
  this->_message.getBytes(_message_buffer_ptr, length, offset);
}

void LoggrProvider::_send_message() {
  const unsigned int length = _message.length();
  if (length <= LOGGR_MESSAGE_BUFFER_SIZE) {
    _message_buffer_length = length;
    _message.getBytes(&_message_buffer[0], length);
    _flush();
    return;
  }
  uint8_t chunks_count = (length + LOGGR_MESSAGE_BUFFER_SIZE - 1) / LOGGR_MESSAGE_BUFFER_SIZE;
  unsigned int offset;
  unsigned int chunk_length;
  for (uint8_t i = 0; i < chunks_count; i++) {
    offset = i * LOGGR_MESSAGE_BUFFER_SIZE;
    if (offset + LOGGR_MESSAGE_BUFFER_SIZE <= length) {
      _message_buffer_length = LOGGR_MESSAGE_BUFFER_SIZE;
    } else {
      _message_buffer_length = length - offset;
    }
    _message.getBytes(_message_buffer_ptr, _message_buffer_length, offset);
    _flush();
    if (_result == 0) {
      _connected = false;
    }
  }
}

LoggrProvider Loggr;
