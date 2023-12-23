#pragma once

#include <functional>
#include <AsyncUDP.h>
#include <mutex>
#include "SocketClient.h"

#define SOCKET_RESPONSE_MAX_LENGTH 64
#define SOCKET_RESPONSE_PADDING 3
#define SocketReplyHandler std::function<uint8_t(SocketResponse *resp)>

typedef uint8_t MessageLength;

class SocketResponse {
  public:
    bool sent = false;

    SocketResponse(
      uint8_t domain,
      uint8_t command,
      ClientIndex index,
      SocketClient *clients,
      AsyncUDP *udp
    );

    bool append(uint8_t data);
    void send_boolean(bool value);
    void reply(SocketReplyHandler handle);

  private:
    uint8_t _command;
    uint8_t _message[SOCKET_RESPONSE_MAX_LENGTH];
    MessageLength _message_length = 0;
    ClientIndex _index;
    SocketClient *_clients;
    AsyncUDP *_udp;

    void _set_length();
    void _send(ClientIndex i);
    void _send();
    void _clear();
};
