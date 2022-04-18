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
    _connect_attempts--;
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

bool Lightstrip::setColor(uint8_t r, uint8_t g, uint8_t b) {
  _color.r = r;
  _color.g = g;
  _color.b = b;
  _mode = MODE_RGB;
  _enabled = true;
  return _data->send(COMMAND_SET_COLOR, r, g, b);
}

bool Lightstrip::setBrightness(uint8_t brightness) {
  _brightness = brightness;
  return _data->send(COMMAND_SET_BRIGHTNESS, brightness);
}

bool Lightstrip::setTemperature(uint8_t temperature) {
  _temperature = temperature;
  _mode = MODE_TEMPERATURE;
  _enabled = true;
  return _data->send(COMMAND_SET_WHITE_TEMPERATURE, temperature);
}

bool Lightstrip::powerOn() {
  _enabled = true;
  return _data->send(COMMAND_SET_BRIGHTNESS, _brightness)
    && _data->send(COMMAND_SET_COLOR, _color.r, _color.g, _color.b);
}

bool Lightstrip::powerOff() {
  _enabled = false;
  return _data->send(COMMAND_SET_BRIGHTNESS, 0)
    && _data->send(COMMAND_SET_COLOR, 0, 0, 0);
}

bool Lightstrip::setTransition(uint16_t transition) {
  return _data->send(COMMAND_SET_TRANSITION, highByte(transition), lowByte(transition));
}

bool Lightstrip::setEffect(uint8_t effect_code) {
  _effect = effect_code;
  return _data->send(COMMAND_SET_EFFECT, _effect);
}

void Lightstrip::_tryConnect() {
  _board->reset();
  _data->connect();
}
