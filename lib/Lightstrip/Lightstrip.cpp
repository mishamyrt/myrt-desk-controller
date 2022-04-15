#include "Lightstrip.h"

void Lightstrip::handle() {
  if (_firmware != NULL) {
    _board->flash(_firmware);
    _firmware->clear();
    _firmware = NULL;
    ESP.restart();
    return;
  }
  _data->handle();
}

void Lightstrip::updateFirmware(FirmwareReader *firmware) {
  _firmware = firmware;
}

/// Handles Dap error
void Lightstrip::onError() {
  Loggr.print("Got dap error");
  if (_connected) {
    _connected = false;
    connect();
  } else if (_connect_attempts > 0) {
    _tryConnect();
  }
}

/// Handles Dap connection
void Lightstrip::onConnect() {
  _connect_attempts = 0;
  _connected = true;
  Loggr.print("Dap connected!");
}

void Lightstrip::connect() {
  _connect_attempts = LIGHTSTRIP_CONNECT_ATTEMPTS;
  _tryConnect();
}

bool Lightstrip::setColor(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
  _brightness = brightness;
  _color.r = r;
  _color.g = g;
  _color.b = b;
  _mode = MODE_RGB;
  _enabled = true;
  return _data->send(COMMAND_SET_COLOR, brightness, r, g, b);
}

bool Lightstrip::setTemperature(uint8_t brightness, uint8_t temperature) {
  _brightness = brightness;
  _temperature = temperature;
  _mode = MODE_TEMPERATURE;
  _enabled = true;
  return _data->send(COMMAND_SET_TEMPERATURE, brightness, temperature);
}

bool Lightstrip::powerOn() {
  _enabled = true;
  return _data->send(COMMAND_POWER_ON, _color.r, _color.g, _color.b);
}

bool Lightstrip::powerOff() {
  _enabled = false;
  return _data->send(COMMAND_POWER_OFF);
}

void Lightstrip::_tryConnect() {
  _board->reset();
  _data->connect();
}
