#pragma once

#include "stdint.h"
#include "Stream.h"
#include "DapRequest.h"
#include "DapController.h"
#include "include/FIFO.h"

#define DAP_BUFFER_SIZE 16
#define DAP_INTERVAL 300
#define DAP_STACK_SIZE 8
#define DAP_TIMEOUT 2000

class Dap {
  public:
    Dap(Stream *connection) {
      _serial = connection;
    }

    bool isConnected() {
      return _connected;
    }

    void setController(DapController *controller) {
      this->controller = controller;
    }

    void connect();
    void handle();

    bool send(uint8_t *payload, uint8_t payload_length);
    bool send(uint8_t command);
    bool send(uint8_t command, uint8_t first);
    bool send(uint8_t command, uint8_t first, uint8_t second);
    bool send(uint8_t command, uint8_t first, uint8_t second, uint8_t third);
    bool send(uint8_t command, uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
    bool send(uint8_t command, uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t fifth);

  private:
    DapController *controller;
    Stream *_serial;
    uint8_t _input_buffer[DAP_BUFFER_SIZE];
    FIFORing<DapRequest *, DAP_STACK_SIZE> _requests;
    unsigned long _next_request_time = millis();
    bool _connected;
    bool _started;

    void _onError();
    void _clear();
    void _deleteRequest(DapRequest *request);
    void _handleRequest(DapRequest *request);
    void _sendMessage(DapRequest *request);
    void _readResponse(DapRequest *request);
    bool _addRequest(DapRequest *request);
    bool _readStauts();
};
