// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <AsyncUDP.h>

#define LOGGR_MESSAGE_BUFFER_SIZE 256
#define LOGGR_PRESERVE_LOGS true

class LoggrProvider {
 public:
  void setProtocol(AsyncUDP *udp);

  void setClient(AsyncUDPPacket *client);

  void send();

  void print(String content);

 private:
  AsyncUDP *_udp;
  uint16_t _remote_port = 0;
  IPAddress _remote_ip;
  String _message = "";
  unsigned char _message_buffer[128];
  unsigned int _message_buffer_length = 0;
  bool _connected = false;
  unsigned char *_message_buffer_ptr = &_message_buffer[0];
  size_t _result = 0;

  void _flush();
  void _send_message();
  void _fillBuffer(const unsigned int length, const unsigned int offset);
};

extern LoggrProvider Loggr;
