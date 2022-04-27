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
    void begin(Stream *serial);
    void handle();
    bool handleMessage(uint8_t *message, uint8_t length);
    CommandServer& on(uint8_t code, bool (*handler)(uint8_t *message, uint8_t length));

  private:
    CommandHandler _handlers[32];
    uint8_t _handlers_count = 0;
    DapSlave _data = DapSlave();
};
