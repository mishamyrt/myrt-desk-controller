// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <AsyncUDP.h>

#define LOGGR_MESSAGE_BUFFER_SIZE 128
class LoggrProvider {
 public:
  String message;

  void setProtocol(AsyncUDP *udp);

  void setClient(AsyncUDPPacket *client);

  void send();

  void print(String content);

 private:
  AsyncUDP *_udp;
  bool _connected = false;
  uint16_t _remote_port = 0;
  IPAddress _remote_ip;
  unsigned char _message_buffer[128];
  unsigned char *_message_buffer_ptr = &_message_buffer[0];
};

extern LoggrProvider Loggr;
