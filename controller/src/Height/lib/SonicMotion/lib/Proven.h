#pragma once

#include <stdint.h>

/// Stores and updates values that need to be proven
/// This means that the value must be confirmed several times before it is saved
template <typename T>
class Proven {
  public:
    Proven(T initial_value) {
      _value = initial_value;
    }

    bool set(T value, uint8_t proves_count) {
      if (value == _value) {
        return false;
      }
      if (value == _target_value) {
        _proves_count++;
        if (_proves_count >= proves_count) {
          _value = _target_value;
          return true;
        }
      } else {
        _target_value = value;
        _proves_count = 1;
      }
      return false;
    }

    T get() {
      return _value;
    }

  private:
    T _value;
    T _target_value;
    uint8_t _proves_count;

};
