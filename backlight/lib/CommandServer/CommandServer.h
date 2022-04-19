#pragma once

#include <stdint.h>
#include <Stream.h>
#include "lib/DapSlave/DapSlave.h"

struct CommandHandler {
  uint8_t code;
  bool (*handle)(uint8_t *message, uint8_t length);
};

// Command wrapper around Dap slave
class CommandServer : public DapHandler {
  public:
    void begin(Stream *serial) {
      _data.setHandler(this);
      _data.connect(serial);
    }

    CommandServer& on(uint8_t code, bool (*handler)(uint8_t *message, uint8_t length)) {
      _handlers[_handlers_count].code = code;
      _handlers[_handlers_count].handle = handler;
      _handlers_count++;
      return *this;
    }

    void handle() {
      _data.handle();
    }

    bool handleMessage(uint8_t *message, uint8_t length) {
      for (uint8_t i = 0; i < _handlers_count; i++) {
        if (_handlers[i].code == message[0]) {
          return _handlers[i].handle(message, length);
        }
      }
      return false;
    }

  private:
    CommandHandler _handlers[32];
    uint8_t _handlers_count = 0;
    DapSlave _data = DapSlave();
};
