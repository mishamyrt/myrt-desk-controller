// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>
#include <Loggr.h>
#include <Store.h>
#include "lib/DapMaster/DapMaster.h"
#include "lib/AVRLord/AVRLord.h"
#include "constants.h"

#define LIGHTSTRIP_CONNECT_ATTEMPTS 3

struct RGB {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
};

struct BacklightState {
  ColorMode mode = MODE_RGB;
  RGB color = RGB{0, 0, 0};
  uint8_t temperature = 0;
  uint8_t brightness = 0;
  uint16_t transition = 0;
  uint8_t effect = 0;
  bool enabled = false;
};

class BacklightController : public DapController {
 public:
  BacklightController(DapMaster *connection, AVRLord *board);

  // State getters
  ColorMode mode() {
    return _state.mode;
  }
  RGB color() {
    return _state.color;
  }
  uint8_t temperature() {
    return _state.temperature;
  }
  uint8_t brightness() {
    return _state.brightness;
  }
  bool enabled() {
    return _state.enabled;
  }
  uint8_t effect() {
    return _state.effect;
  }

  // DapController hooks
  void onError();
  void onConnect();

  void connect();
  void handle();
  void updateFirmware(FirmwareReader *reader);

  // State setter
  bool setColor(uint8_t r, uint8_t g, uint8_t b);
  bool setBrightness(uint8_t brightness);
  bool setTemperature(uint8_t temperature);
  bool setTransition(uint16_t transition);
  bool setEffect(uint8_t effect_code);
  bool powerOn();
  bool powerOff();


 private:
  // Internal variables
  bool _connected;
  uint8_t _connect_attempts;

  // External connections
  DapMaster *_data;
  AVRLord *_board;

  BacklightState _state;
  Stored _descriptor = Stored(_state);

  FirmwareReader *_firmware = NULL;

  void _tryConnect();
  void _applyState();
};
