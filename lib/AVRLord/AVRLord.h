#pragma once

#include "Arduino.h"
#include "FirmwareReader.h"
#include "Loggr.h"
#include "./include/commands.h"

class AVRUpdater {
  public:
    virtual void handleUpdateDone(bool is_success) = 0;
};

class AVRLord {
  public:
    uint8_t input_buffer[5];
    FirmwareReader *reader;

    AVRLord(uint8_t reset_pin) {
      this->reset_pin = reset_pin;
      pinMode(reset_pin, OUTPUT);
      digitalWrite(reset_pin, HIGH);
    }

    void flash(FirmwareReader *reader) {
      Serial.flush();
      Serial.end();
      Serial.begin(57600);
      this->reader = reader;
      address = 0;
      data_idx = 0;
      flashAVR();
    }

    void setUpdater(AVRUpdater *updater) {
      this->updater = updater;
    }

    bool flashAVR() {
      reset();
      if (!getSync()) {
        return false;
      }
      if (!activateProgMode()) {
        return false;
      }
      if (!readSign()) {
        return false;
      }
      while (data_idx <= reader->size()) {
        if (!writePage()) {
          return false;
        }
        address += 0x40;
        delay(10);
      }
      leaveProgMode();
      delay(250);
      reset();
      return true;
    }

    void reset() {
      // Activate reset pin
      digitalWrite(reset_pin, LOW);
      delay(50);
      digitalWrite(reset_pin, HIGH);
      delay(200);
      // Skip start junk
      clearSerialBuffer();
    }

  private:
    uint8_t reset_pin;
    unsigned long address = 0;
    size_t data_idx = 0;
    AVRUpdater *updater;

    bool requestPageWriting() {
      Serial.write(STK_LOAD_ADDRESS);
      Serial.write(lowByte(address));
      Serial.write(highByte(address));
      Serial.write(SYNC_CRC_EOP);
      return readSync();
    }

    bool writePage() {
      if (!requestPageWriting()) {
        return false;
      }
      Serial.write(STK_PROGRAM_PAGE);
      Serial.write(0x00);
      Serial.write(PAGE_SIZE);
      Serial.write(MEMORY_TYPE_FLASH);
      for (uint8_t i = 0; i < PAGE_SIZE; i++) {
        Serial.write(reader->valueAt(data_idx++));
      }
      Serial.write(SYNC_CRC_EOP);
      Serial.flush();
      return readSync();
    }

    bool readSign() {
      Serial.write(STK_READ_SIGN);
      Serial.write(SYNC_CRC_EOP);
      return read(5) && input_buffer[4] == STK_OK;
    }

    bool getSync() {
      Serial.write(STK_GET_SYNCH);
      Serial.write(SYNC_CRC_EOP);
      return readSync();
    }

    bool leaveProgMode() {
      Serial.write(STK_LEAVE_PROGMODE);
      Serial.write(SYNC_CRC_EOP);
      return readSync();
    }

    bool activateProgMode() {
      Serial.write(STK_ENTER_PROGMODE);
      Serial.write(SYNC_CRC_EOP);
      return readSync();
    }

    bool readSync() {
      if (!read(2)) {
        return false;
      }
      return input_buffer[0] == STK_INSYNC && input_buffer[1] == STK_OK;
    }

    bool read(uint8_t count) {
      uint8_t attempts = 30;
      while (attempts > 0 && Serial.available() < count) {
        delay(3);
        attempts--;
      }
      if (attempts == 0) {
        return false;
      }
      Serial.readBytes(input_buffer, count);
      return true;
    }

    void clearSerialBuffer() {
      Serial.flush();
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
};
