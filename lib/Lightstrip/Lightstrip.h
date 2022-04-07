#pragma once

#include "Arduino.h"
#include "Blink.h"

typedef uint8_t RGB[3];

/// Dap lightstrip integration
class Lightstrip {
  public:
    Lightstrip(uint8_t pin) {
      reset_pin = pin;
    }

    // Let the state variables be public, too lazy to make getters.
    bool enabled;
    uint8 brightness;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    /// Setup serial connection with Dap enabled device on UART2
    void initialize(unsigned long baud = 115200) {

      pinMode(reset_pin, OUTPUT);
      digitalWrite(reset_pin, HIGH);
      Serial.begin(baud);
      Serial.flush();
      Serial.swap();
      while (true) {
        reset();
        // Wait for start junk and clear
        delay(50);
        clearBuffer();
        waitForMessage();
        if (readStatus()) {
          return;
        }
        blink(2);
      }
      // Reset controller and wait for initialization code
    }

    bool setEffect(uint8_t effect) {
      return writeMessage(2, COMMAND_START_EFFECT, effect);
    }

    bool setColor(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
      this->enabled = true;
      this->brightness = brightness;
      this->r = r;
      this->g = g;
      this->b = b;
      return writeMessage(5, COMMAND_SET_COLOR, brightness, r, g, b);
    }

    bool powerOff() {
      enabled = false;
      return writeMessage(1, COMMAND_POWER_OFF);
    }

    bool powerOn() {
      enabled = true;
      if (r == 0 && g == 0 && b == 0) {
        r = 255;
        g = 255;
        b = 255;
      }
      return writeMessage(4, COMMAND_POWER_ON, r, g, b);
    }

  private:
    uint8_t reset_pin;
    uint8_t message_buffer[128];
    uint8_t message_length;

    enum {
      COMMAND_POWER_OFF = 0,
      COMMAND_POWER_ON = 1,
      COMMAND_SET_COLOR = 2,
      COMMAND_SET_COLOR_ZONES = 3,
      COMMAND_START_EFFECT = 4
    };

    enum {
      CODE_SUCCESS = 0xEE,
      CODE_ERROR = 0xFF
    };

    void reset() {
      digitalWrite(reset_pin, LOW);
      delay(15);
      digitalWrite(reset_pin, HIGH);
    }

    void clearBuffer() {
      while (Serial.read() != -1) {}
    }

    void waitForMessage() {
      while (Serial.available() < 4) {}
    }

    bool readStatus() {
      if (Serial.available() < 4) {
        return false;
      }
      Serial.read(); // First header
      Serial.read(); // Second header
      Serial.read(); // Length
      int data = Serial.read();
      switch (data) {
        case CODE_SUCCESS:
          return true;
        case CODE_ERROR:
        case -1:
          return false;
      }
      clearBuffer();
      return false;
    }

    bool writeMessage(uint8_t length, ...) {
      Serial.write(0xFE);
      Serial.write(0xEF);
      Serial.write(length);
      va_list ap;
      va_start(ap, length);
      for (uint8_t i = 0; i < length; i++) {
        Serial.write(va_arg(ap, int));
      }
      va_end(ap);
      waitForMessage();
      return readStatus();
    }
};
