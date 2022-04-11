#pragma once

#include "Arduino.h"

#define PAGE_SIZE 128

class FirmwareReader {
  public:
    size_t size() {
      return _size;
    }

    uint8_t valueAt(size_t idx) {
      if (idx >= _size) {
        return 0;
      }
      return data[idx];
    }

    void create(size_t size) {
      delete[] data;
      _size = size;
      data = new uint8_t[size];
    }

    void clear() {
      delete[] data;
      _size = 0;
    }

    void appendChunk (uint8_t *data, size_t length, size_t index) {
      for (size_t i = 0; i < length; i++) {
        this->data[i + index] = data[i];
      }
    }

  private:
    size_t _size;
    uint8_t *data;
};
