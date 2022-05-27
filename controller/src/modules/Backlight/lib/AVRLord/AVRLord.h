// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

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

  explicit AVRLord(uint8_t reset_pin) {
    this->reset_pin = reset_pin;
    pinMode(reset_pin, OUTPUT);
    digitalWrite(reset_pin, HIGH);
  }

  void flash(FirmwareReader *reader) {
    Serial2.flush();
    Serial2.end();
    Serial2.begin(57600);
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
    clearSerial2Buffer();
  }

 private:
  uint8_t reset_pin;
  uint32_t address = 0;
  size_t data_idx = 0;
  AVRUpdater *updater;

  bool requestPageWriting() {
    Serial2.write(STK_LOAD_ADDRESS);
    Serial2.write(lowByte(address));
    Serial2.write(highByte(address));
    Serial2.write(SYNC_CRC_EOP);
    return readSync();
  }

  bool writePage() {
    if (!requestPageWriting()) {
      return false;
    }
    Serial2.write(STK_PROGRAM_PAGE);
    Serial2.write(0x00);
    Serial2.write(PAGE_SIZE);
    Serial2.write(MEMORY_TYPE_FLASH);
    for (uint8_t i = 0; i < PAGE_SIZE; i++) {
      Serial2.write(reader->valueAt(data_idx++));
    }
    Serial2.write(SYNC_CRC_EOP);
    Serial2.flush();
    return readSync();
  }

  bool readSign() {
    Serial2.write(STK_READ_SIGN);
    Serial2.write(SYNC_CRC_EOP);
    return read(5) && input_buffer[4] == STK_OK;
  }

  bool getSync() {
    Serial2.write(STK_GET_SYNCH);
    Serial2.write(SYNC_CRC_EOP);
    return readSync();
  }

  bool leaveProgMode() {
    Serial2.write(STK_LEAVE_PROGMODE);
    Serial2.write(SYNC_CRC_EOP);
    return readSync();
  }

  bool activateProgMode() {
    Serial2.write(STK_ENTER_PROGMODE);
    Serial2.write(SYNC_CRC_EOP);
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
    while (attempts > 0 && Serial2.available() < count) {
      delay(3);
      attempts--;
    }
    if (attempts == 0) {
      return false;
    }
    Serial2.readBytes(input_buffer, count);
    return true;
  }

  void clearSerial2Buffer() {
    Serial2.flush();
    while (Serial2.available() > 0) {
      Serial2.read();
    }
  }
};
