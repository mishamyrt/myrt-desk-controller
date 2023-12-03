#pragma once

#include <functional>
#include <AsyncUDP.h>
#include <mutex>
#include "SocketClient.h"
#include "SocketMessageHandler.h"

class SocketServer {
  public:
    void attach(AsyncUDP *server);
    void handle(AsyncUDPPacket *packet);
    void setHandler(SocketMessageHandler *handler);

  private:
    AsyncUDP *_udp;
    SocketMessageHandler *_handler;
    SocketClient _clients[SOCKET_SERVER_MAX_CLIENTS];

    ClientIndex _addClient(AsyncUDPPacket *packet);
    int _indexOfClient(AsyncUDPPacket *packet);
};
