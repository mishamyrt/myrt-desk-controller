#pragma once

#include <EEPROM.h>
#include <Arduino.h>
#include <Timer.h>
#include "Stored.h"

#define STORE_SIZE 256
#define STORE_COUNT 16
#define STORE_UPDATE_INTERVAL 5000

class StoreController {
  public:
    StoreController();
    void handle();
    bool isCorrect(Stored *value);
    void load(Stored *value);
    void write(Stored *value);
    bool append(Stored *value);

  private:
    Stored *_descriptors[STORE_COUNT];
    Timer _update_interval;
    uint8_t _count = 0;
    uint16_t _next_addr = 0;
};
