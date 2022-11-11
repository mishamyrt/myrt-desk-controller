// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "DapMaster.h"
#include "DapRequest.h"
#include "include/FIFO.h"
#include "include/codes.h"

/// Async connect to device. Response will be sent to *controller
void DapMaster::connect() {
  // Creating fake request with sent state and increased timeout
  // It will cause status read on next step
  _started = false;
  DapRequest *status_request = new DapRequest();
  status_request->state = D_SENT;
  status_request->payload = NULL;
  status_request->payload_length = 0;
  status_request->expiry = millis() + 3000;
  _addRequest(status_request);
}

/// Sync entry point. Handles requests
void DapMaster::handle() {
  if ((_started && !_connected) || _requests.isEmpty()) {
    return;
  }
  DapRequest *request = _requests.peek();
  _handleRequest(request);
}

/// Sends message to connected device
bool DapMaster::send(uint8_t *payload, uint8_t payload_length) {
  if (!_connected) {
    return false;
  }
  DapRequest *request = new DapRequest();
  request->state = D_CREATED;
  request->payload = payload;
  request->payload_length = payload_length;
  return _addRequest(request);
}

bool DapMaster::send(uint8_t command) {
  uint8_t *message = new uint8_t[1];
  message[0] = command;
  return send(message, 1);
}

bool DapMaster::send(uint8_t command, uint8_t first) {
  uint8_t *message = new uint8_t[2];
  message[0] = command;
  message[1] = first;
  return send(message, 2);
}

bool DapMaster::send(uint8_t command, uint8_t first, uint8_t second) {
  uint8_t *message = new uint8_t[3];
  message[0] = command;
  message[1] = first;
  message[2] = second;
  return send(message, 3);
}

bool DapMaster::send(
  uint8_t command,
  uint8_t first,
  uint8_t second,
  uint8_t third
) {
  uint8_t *message = new uint8_t[4];
  message[0] = command;
  message[1] = first;
  message[2] = second;
  message[3] = third;
  return send(message, 4);
}

bool DapMaster::send(
  uint8_t command,
  uint8_t first,
  uint8_t second,
  uint8_t third,
  uint8_t fourth
) {
  uint8_t *message = new uint8_t[5];
  message[0] = command;
  message[1] = first;
  message[2] = second;
  message[3] = third;
  message[4] = fourth;
  return send(message, 5);
}

bool DapMaster::send(
  uint8_t command,
  uint8_t first,
  uint8_t second,
  uint8_t third,
  uint8_t fourth,
  uint8_t fifth
) {
  uint8_t *message = new uint8_t[6];
  message[0] = command;
  message[1] = first;
  message[2] = second;
  message[3] = third;
  message[4] = fourth;
  message[5] = fifth;
  return send(message, 6);
}

/// Handles dap request
void DapMaster::_handleRequest(DapRequest *request) {
  switch (request->state) {
    case D_CREATED:
      _sendMessage(request);
      break;
    case D_SENT:
      _readResponse(request);
      break;
    case D_RECIEVED:
      // Move to next request and delete previous
      _deleteRequest(request);
      _requests.next();
      break;
  }
}

/// Reads response from dap device
void DapMaster::_readResponse(DapRequest *request) {
  if (millis() >= request->expiry) {
    _onError();
    return;
  }
  if (_serial->available() < 4) {
    return;
  }
  _serial->read();
  _serial->read();
  _serial->read();
  int data = _serial->read();
  // Set interval from last readed symbol
  _next_request_time = millis() + DAP_INTERVAL;
  switch (data) {
    case DAP_SUCCESS:
      if (!_connected) {
        _connected = true;
      }
      if (!_started) {
        _started = true;
        controller->onConnect();
      }
      break;
    case DAP_ERROR:
    case -1:
      _onError();
      break;
  }
  request->state = D_RECIEVED;
}

/// Sends message to dap device
void DapMaster::_sendMessage(DapRequest *request) {
  if (millis() < _next_request_time) {
    return;
  }
  _serial->write(DAP_HEADER_FIRST);
  _serial->write(DAP_HEADER_SECOND);
  _serial->write(request->payload_length);
  for (uint8_t i = 0; i < request->payload_length; i++) {
    _serial->write(request->payload[i]);
  }
  _serial->flush();
  request->state = D_SENT;
  if (request->expiry == 0) {
    request->expiry = millis() + DAP_TIMEOUT;
  }
}

/// Adds request to queue
bool DapMaster::_addRequest(DapRequest *request) {
  return _requests.append(request);
}

void DapMaster::_clear() {
  while (!_requests.isEmpty()) {
    _deleteRequest(
      _requests.next()
    );
  }
}

void DapMaster::_deleteRequest(DapRequest *request) {
  if (request == NULL) return;
  if (request->payload != NULL) {
    delete [] request->payload;
  }
  delete request;
}

void DapMaster::_onError() {
  _clear();
  _connected = false;
  controller->onError();
}
