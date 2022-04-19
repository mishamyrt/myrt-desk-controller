#pragma once

#include <stdint.h>

template <typename T, int SIZE>
class FIFORing {
  public:
    bool append(T value) {
      uint8_t i = (_head + 1) % SIZE;
      if (i == _tail) {
        return false; // Out of place
      }
      _buffer[_head] = value;
      _head = i;
      return true;
    }

    bool isEmpty() {
      return _head == _tail;
    }

    T next() {
      if (isEmpty()) {
        return 0;
      }
      T value = _buffer[_tail];
      _tail = (_tail + 1) % SIZE;
      return value;
    }

    T peek() {
      return _buffer[_tail];
    }

    uint8_t available() {
        return (SIZE + _head - _tail) % SIZE;
    }

    void clear() {
      _head = 0;
      _tail = 0;
    }

  private:
    T _buffer[SIZE];
    uint8_t _head = 0;
    uint8_t _tail = 0;
};
