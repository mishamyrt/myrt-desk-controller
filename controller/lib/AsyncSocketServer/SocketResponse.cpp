#include "SocketResponse.h"

std::mutex socketWriteLock;

SocketResponse::SocketResponse(
  uint8_t domain,
  uint8_t command,
  ClientIndex index,
  SocketClient *clients,
  AsyncUDP *udp
) {
  _message[1] = domain;
  _message[2] = command;
  _index = index;
  _clients = clients;
  _udp = udp;
}

bool SocketResponse::append(uint8_t data) {
  // The zero element stores the length. Clients will expect response in this format
  if (_message_length >= SOCKET_RESPONSE_MAX_LENGTH - SOCKET_RESPONSE_PADDING) {
    return false;
  }
  _message[_message_length + SOCKET_RESPONSE_PADDING] = data;
  _message_length += 1;
  return true;
}

void SocketResponse::sendBoolean(bool value) {
  _message_length = 1;
  _message[SOCKET_RESPONSE_PADDING] = value ? 0 : 1;
  flush();
}

void SocketResponse::flush() {
  _set_length();
  _send(_index);
  sent = true;
}

void SocketResponse::broadcast() {
  _set_length();
  for (ClientIndex i = 0; i < SOCKET_SERVER_MAX_CLIENTS; i++) {
    if (!_clients[i].active) {
      continue;
    }
    _clients[i].active = _send(i);
  }
  sent = true;
}

void SocketResponse::_set_length() {
  _message[0] = _message_length + SOCKET_RESPONSE_PADDING - 1;
}

bool SocketResponse::_send(ClientIndex i) {
  std::lock_guard<std::mutex> lck(socketWriteLock);
  return _udp->writeTo(
    &_message[0],
    _message_length + SOCKET_RESPONSE_PADDING,
    _clients[i].ip,
    _clients[i].port
  ) > 0;
}
