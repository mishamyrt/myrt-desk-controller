// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <EEPROM.h>
#include <Arduino.h>
#include <Timer.h>
#include "Stored.h"

#define STORE_SIZE 512
#define STORE_COUNT 16
#define STORE_UPDATE_INTERVAL 5000

class StoreController {
 public:
  void handle();
  void initialize();
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
