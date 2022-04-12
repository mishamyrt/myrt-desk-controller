#pragma once

#include "Arduino.h"
#include "Blink.h"
#include "AVRLord.h"
#include "FirmwareReader.h"
#include "OTAListener.h"
#include "OTA.h"

typedef uint8_t RGB[3];

/// Dap lightstrip integration
class Lightstrip : public AVRUpdater, public OTAListener {
  public:
    Lightstrip(AVRLord *avr) {
      this->avr = avr;
      avr->setUpdater(this);
      OTA.setListener(this);
    }

    void onOTAStart() {
      writePowerOff();
    }

    void handle() {
      switch (action) {
        case ACTION_SET_COLOR:
          writeMessage(5, COMMAND_SET_COLOR, brightness, r, g, b);
          break;
        case ACTION_SET_EFFECT:
          writeMessage(2, COMMAND_START_EFFECT, effect);
          break;
        case ACTION_UPDATE:
          onOTAStart();
          action = ACTION_NONE;
          avr->flash(reader);
          return;
        case ACTION_POWER_OFF:
          writePowerOff();
          break;
        case ACTION_POWER_ON:
          writeMessage(4, COMMAND_POWER_ON, r, g, b);
          break;
        case ACTION_SET_TEMPERATURE:
          writeMessage(3, COMMAND_SET_TEMPERATURE, brightness, temperature);
          break;
      }
      action = ACTION_NONE;
    }

    bool writePowerOff() {
      return writeMessage(1, COMMAND_POWER_OFF);
    }

    void handleUpdateDone(bool is_success) {
      if (is_success && initialize()) {
        ESP.reset();
      }
    }

    // Let the state variables be public, too lazy to make getters.
    bool enabled;
    uint8 brightness;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t effect;
    uint8_t temperature;

    /// Setup serial connection with Dap enabled device on UART
    bool initialize() {
      uint8_t attempts = 4;
      while (attempts > 0) {
        avr->reset();
        delay(1000);
        if (hasMessage() && readStatus()) {
          return true;
        }
        Serial.flush();
        attempts--;
      }
      return false;
    }

    bool updateFirmware(FirmwareReader *reader) {
      if (!isReady()) {
        return false;
      }
      this->reader = reader;
      action = ACTION_UPDATE;
      return true;
    }

    bool setEffect(uint8_t effect) {
      if (!isReady()) {
        return false;
      }
      this->effect = effect;
      action = ACTION_SET_EFFECT;
      return true;
    }

    bool setTemperature(uint8_t brightness, uint8_t temperature) {
      if (!isReady()) {
        return false;
      }
      this->enabled = true;
      this->brightness = brightness;
      this->temperature = temperature;
      action = ACTION_SET_TEMPERATURE;
      return true;
    }

    bool setColor(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
      if (!isReady()) {
        return false;
      }
      this->enabled = true;
      this->brightness = brightness;
      this->r = r;
      this->g = g;
      this->b = b;
      action = ACTION_SET_COLOR;
      return true;
    }

    bool powerOff() {
      enabled = false;
      action = ACTION_POWER_OFF;
      return true;
    }

    bool powerOn() {
      enabled = true;
      if (r == 0 && g == 0 && b == 0) {
        r = 255;
        g = 255;
        b = 255;
      }
      action = ACTION_POWER_ON;
      return true;
    }

  private:
    AVRLord *avr;
    FirmwareReader *reader;
    uint8_t message_buffer[128];
    uint8_t message_length;
    uint8_t action = ACTION_NONE;

    enum {
      COMMAND_POWER_OFF = 0,
      COMMAND_POWER_ON = 1,
      COMMAND_SET_COLOR = 2,
      COMMAND_SET_COLOR_ZONES = 3,
      COMMAND_START_EFFECT = 4,
      COMMAND_SET_TEMPERATURE = 5,
    };
    // TODO: ↓ Merge this enums ↑
    enum {
      ACTION_NONE,
      ACTION_SET_COLOR,
      ACTION_SET_EFFECT,
      ACTION_POWER_ON,
      ACTION_POWER_OFF,
      ACTION_UPDATE,
      ACTION_SET_TEMPERATURE,
    };

    enum {
      CODE_SUCCESS = 0xEE,
      CODE_ERROR = 0xFF,
    };

    void clearBuffer() {
      while (Serial.read() != -1) {}
    }

    bool isReady() {
      return action == ACTION_NONE;
    }

    bool hasMessage() {
      uint8_t attempts = 255;
      while (Serial.available() < 4) {
        attempts--;
        if (attempts == 0) {
          return false;
        }
        delay(2);
      }
      return true;
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
      if (!hasMessage()) {
        return false;
      }
      return readStatus();
    }
};
