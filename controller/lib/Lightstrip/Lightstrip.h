#pragma once

#include "Arduino.h"
#include "DapMaster.h"
#include "DapController.h"
#include "AVRLord.h"
#include "FirmwareReader.h"
#include "Loggr.h"
#include "constants.h"

#define LIGHTSTRIP_CONNECT_ATTEMPTS 3

struct RGB {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
};

struct LightstripState {
  ColorMode mode;
  RGB color;
  uint8_t temperature;
  uint8_t brightness;
  uint16_t transition;
  uint8_t effect;
};

class Lightstrip : public DapController {
  public:
    Lightstrip(DapMaster *connection, AVRLord *board) {
      _data = connection;
      _data->setController(this);
      _board = board;
    }

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
      return _enabled;
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

    LightstripState _state;

    bool _enabled = false;
    FirmwareReader *_firmware = NULL;

    void _tryConnect();
};
