#include "SocketServer.h"

std::mutex clientChangeLock;

void SocketServer::attach(AsyncUDP *server) {
  _udp = server;
  for (ClientIndex i = 0; i < SOCKET_SERVER_MAX_CLIENTS; i++) {
    _clients[i].active = false;
  }
}

void SocketServer::handle(AsyncUDPPacket *packet) {
  uint8_t *message = packet->data();
  size_t length = packet->length();
  size_t payload_length = message[0];
  if (payload_length < 2 || payload_length != length - 1) {
    return;
  }
  int clientIndex = _indexOfClient(packet);
  if (clientIndex == -1) {
    clientIndex = _addClient(packet);
  }
  _clients[clientIndex].updatedAt = millis();

  SocketResponse *resp = new SocketResponse(
    message[1],
    message[2],
    clientIndex,
    &_clients[0],
    _udp
  );

  bool success = false;
  if (_handler != NULL) {
    success = _handler->handle(&message[1], payload_length, resp);
  }
  resp->sendBoolean(success);
  delete[] resp;
}

void SocketServer::setHandler(SocketMessageHandler *handler) {
  _handler = handler;
}

ClientIndex SocketServer::_addClient(AsyncUDPPacket *packet) {
  std::lock_guard<std::mutex> lck(clientChangeLock);
  for (ClientIndex i = 0; i < SOCKET_SERVER_MAX_CLIENTS; i++) {
    unsigned long timeDiff = millis() - _clients[i].updatedAt;
    if (!_clients[i].active || timeDiff > SOCKET_SERVER_CLIENT_TIMEOUT) {
      _clients[i].ip = packet->remoteIP();
      _clients[i].port = packet->remotePort();
      _clients[i].active = true;
      _clients[i].updatedAt = millis();
      return i;
    }
  }
  return -1;
}

int SocketServer::_indexOfClient(AsyncUDPPacket *packet) {
  for (ClientIndex i = 0; i < SOCKET_SERVER_MAX_CLIENTS; i++) {
    IPAddress ip = packet->remoteIP();
    uint16_t port = packet->remotePort();
    if (_clients[i].ip == ip && _clients[i].port == port) {
      if (!_clients[i].active) {
        return -1;
      }
      return i;
    }
  }
  return -1;
}
