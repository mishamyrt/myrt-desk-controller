#pragma once

#include <functional>
#include <Loggr.h>
#include <AsyncUDP.h>
#include "Response.h"

#define DOMAIN_MAX_COMANDS 8

typedef uint8_t Code;

typedef std::function<bool(uint8_t *message, size_t length, CommanderResponse *resp)> AsyncCommandHandler;

struct Command {
  Code code;
  AsyncCommandHandler handle;
};

class Domain {
  public:
    Code code;

    bool on(uint8_t code, AsyncCommandHandler handle);

    bool handle(uint8_t *message, size_t length, CommanderResponse *resp);

  private:
    Command _commands[DOMAIN_MAX_COMANDS];
    uint8_t _head = 0;
};
