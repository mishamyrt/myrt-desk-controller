// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Loggr.h"

void LoggrProvider::setProtocol(AsyncUDP *udp) {
  _udp = udp;
}

void LoggrProvider::setClient(AsyncUDPPacket *client) {
  _remote_ip = client->remoteIP();
  _remote_port = client->remotePort();
  if (_udp != NULL) {
    _connected = true;
  }
}

void LoggrProvider::send() {
  if (_connected) {
    const unsigned int length = this->message.length();
    this->message.getBytes(_message_buffer_ptr, length);
    size_t result = _udp->writeTo(_message_buffer_ptr, length, _remote_ip, _remote_port);
    if (result == 0) {
      _connected = false;
    }
  }
  message = "";
}

void LoggrProvider::print(String content) {
  message = content;
  send();
}

LoggrProvider Loggr;
