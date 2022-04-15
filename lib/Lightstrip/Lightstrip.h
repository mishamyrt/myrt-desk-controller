#pragma once

#include "Arduino.h"
#include "DapMaster.h"
#include "DapController.h"
#include "AVRLord.h"
#include "FirmwareReader.h"
#include "Loggr.h"
#include "./include/constants.h"

#define LIGHTSTRIP_CONNECT_ATTEMPTS 3

struct RGB{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
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
      return _mode;
    }
    RGB color() {
      return _color;
    }
    uint8_t temperature() {
      return _temperature;
    }
    uint8_t brightness() {
      return _brightness;
    }
    bool enabled() {
      return _enabled;
    }

    // DapController hooks
    void onError();
    void onConnect();

    void connect();
    void handle();
    void updateFirmware(FirmwareReader *reader);

    // State setter
    bool setColor(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);
    bool setTemperature(uint8_t brightness, uint8_t temperature);
    bool powerOn();
    bool powerOff();

  private:
    // Internal variables
    bool _connected;
    uint8_t _connect_attempts;

    // External connections
    DapMaster *_data;
    AVRLord *_board;

    // Lightstrip state
    ColorMode _mode = MODE_RGB;
    RGB _color = RGB{0, 0, 0};
    uint8_t _temperature = 0;
    uint8_t _brightness = 0;
    bool _enabled = false;
    FirmwareReader *_firmware = NULL;

    void _tryConnect();
};
