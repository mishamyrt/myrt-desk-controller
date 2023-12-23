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
  _command = command;
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

void SocketResponse::send_boolean(bool value) {
  _clear();
  append(value ? 0 : 1);
  _set_length();
  _send();
}

void SocketResponse::reply(SocketReplyHandler handle) {
  _clear();
  uint8_t command = handle(this);
  _set_length();
  _message[2] = command;
  _send();
}

void SocketResponse::_set_length() {
  _message[0] = _message_length + SOCKET_RESPONSE_PADDING - 1;
}

void SocketResponse::_send(ClientIndex i) {
  std::lock_guard<std::mutex> lck(socketWriteLock);
  SocketClient *client = &_clients[i];
  client->active = _udp->writeTo(
    &_message[0],
    _message_length + SOCKET_RESPONSE_PADDING,
    client->ip,
    client->port
  ) > 0;
}

void SocketResponse::_send() {
  _send(_index);
}

void SocketResponse::_clear() {
  _message_length = 0;
}
