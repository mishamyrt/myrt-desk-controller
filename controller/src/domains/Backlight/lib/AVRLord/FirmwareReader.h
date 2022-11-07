// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "Arduino.h"

#define PAGE_SIZE 128
#define MAX_SIZE 21675

class FirmwareReader {
 public:
  size_t size() {
    return this->_size;
  }

  uint8_t valueAt(size_t idx) {
    if (idx >= this->_size) {
      return 0;
    }
    return this->_data[idx];
  }

  bool create(size_t size) {
    if (size > MAX_SIZE) {
      return false;
    }
    if (this->_data != NULL) {
      delete [] this->_data;
    }
    this->_size = size;
    this->_offset = 0;
    this->_data = new uint8_t[size];
    return true;
  }

  void clear() {
    delete [] this->_data;
    this->_size = 0;
  }

  bool appendChunk (uint8_t *data, size_t length) {
    if (this->_offset + length > this->_size) {
      return false;
    }
    for (size_t i = 0; i < length; i++) {
      this->_data[i + this->_offset] = data[i];
    }
    this->_offset += length;
    return true;
  }

 private:
  size_t _size;
  size_t _offset;
  uint8_t *_data;
};
