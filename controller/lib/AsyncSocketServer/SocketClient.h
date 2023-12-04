#pragma once

#include <AsyncUDP.h>

#define SOCKET_SERVER_MAX_CLIENTS 16
#define SOCKET_SERVER_CLIENT_TIMEOUT 30 * 1000

typedef uint8_t ClientIndex;

struct SocketClient {
  IPAddress ip;
  uint16_t port;
  bool active;
  unsigned long updatedAt;
};
