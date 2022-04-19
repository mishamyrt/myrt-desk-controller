#include "Arduino.h"

/// Simple function, that uses internal LED for
void blink(uint8_t times) {
  for (uint8 i = 0; i < times; i++) {
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
