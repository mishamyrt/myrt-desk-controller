// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "StoreController.h"

StoreController::StoreController() {
  EEPROM.begin(STORE_SIZE);
}

void StoreController::handle() {
  if (_update_interval.isDone()) {
    for (uint8_t i = 0; i < _count; i++) {
      if (_descriptors[i]->shouldUpdate) {
        write(_descriptors[i]);
        _descriptors[i]->shouldUpdate = false;
      }
    }
    _update_interval.set(STORE_UPDATE_INTERVAL);
  }
}

bool StoreController::isCorrect(Stored *value) {
  return EEPROM.read(value->addr + value->addr) != value->key;
}

void StoreController::load(Stored *value) {
  for (uint16_t i = 0; i < value->size; i++) {
    value->data[i] = EEPROM.read(value->addr + i);
  }
}

void StoreController::write(Stored *value) {
  for (uint16_t i = 0; i < value->size; i++) {
    EEPROM.write(value->addr + i, value->data[i]);
  }
  EEPROM.commit();
}

bool StoreController::append(Stored *value) {
  if (_count == STORE_COUNT - 1
    || static_cast<size_t>(_next_addr + value->size + 1) > EEPROM.length()) {
    return false;
  }
  value->addr = _next_addr;
  _next_addr += value->size + 1;
  _descriptors[_count] = value;
  _count++;
  if (isCorrect(value)) {
    load(value);
  } else {
    EEPROM.write(value->addr + value->size, value->key);
    write(value);
  }
  return true;
}
