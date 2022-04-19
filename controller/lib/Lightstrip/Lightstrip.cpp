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
  _state.color.r = r;
  _state.color.g = g;
  _state.color.b = b;
  _state.mode = MODE_RGB;
  _enabled = true;
  return _data->send(COMMAND_SET_COLOR, r, g, b);
}

bool Lightstrip::setBrightness(uint8_t brightness) {
  _state.brightness = brightness;
  return _data->send(COMMAND_SET_BRIGHTNESS, brightness);
}

bool Lightstrip::setTemperature(uint8_t temperature) {
  _state.temperature = temperature;
  _state.mode = MODE_TEMPERATURE;
  _enabled = true;
  return _data->send(COMMAND_SET_WHITE_TEMPERATURE, temperature);
}

bool Lightstrip::powerOn() {
  _enabled = true;
  return _data->send(COMMAND_SET_BRIGHTNESS, _state.brightness)
    && _data->send(COMMAND_SET_COLOR, _state.color.r, _state.color.g, _state.color.b);
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
  _state.effect = effect_code;
  return _data->send(COMMAND_SET_EFFECT, _state.effect);
}

void Lightstrip::_tryConnect() {
  _board->reset();
  _data->connect();
}
