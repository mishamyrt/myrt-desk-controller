#include "Backlight.h"

void Backlight::handle() {
  if (_firmware != NULL) {
    _board->flash(_firmware);
    _firmware->clear();
    _firmware = NULL;
    ESP.restart();
    return;
  }
  _data->handle();
}

void Backlight::updateFirmware(FirmwareReader *firmware) {
  _firmware = firmware;
}

/// Handles Dap error
void Backlight::onError() {
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
void Backlight::onConnect() {
  _connect_attempts = 0;
  _connected = true;
  Loggr.print("Dap connected!");
}

void Backlight::connect() {
  _connect_attempts = LIGHTSTRIP_CONNECT_ATTEMPTS;
  _tryConnect();
}

bool Backlight::setColor(uint8_t r, uint8_t g, uint8_t b) {
  _state.color.r = r;
  _state.color.g = g;
  _state.color.b = b;
  _state.mode = MODE_RGB;
  _enabled = true;
  return _data->send(COMMAND_SET_COLOR, r, g, b);
}

bool Backlight::setBrightness(uint8_t brightness) {
  _state.brightness = brightness;
  return _data->send(COMMAND_SET_BRIGHTNESS, brightness);
}

bool Backlight::setTemperature(uint8_t temperature) {
  _state.temperature = temperature;
  _state.mode = MODE_TEMPERATURE;
  _enabled = true;
  return _data->send(COMMAND_SET_WHITE_TEMPERATURE, temperature);
}

bool Backlight::powerOn() {
  _enabled = true;
  return _data->send(COMMAND_SET_BRIGHTNESS, _state.brightness)
    && _data->send(COMMAND_SET_COLOR, _state.color.r, _state.color.g, _state.color.b);
}

bool Backlight::powerOff() {
  _enabled = false;
  return _data->send(COMMAND_SET_BRIGHTNESS, 0)
    && _data->send(COMMAND_SET_COLOR, 0, 0, 0);
}

bool Backlight::setTransition(uint16_t transition) {
  return _data->send(COMMAND_SET_TRANSITION, highByte(transition), lowByte(transition));
}

bool Backlight::setEffect(uint8_t effect_code) {
  _state.effect = effect_code;
  return _data->send(COMMAND_SET_EFFECT, _state.effect);
}

void Backlight::_tryConnect() {
  _board->reset();
  _data->connect();
}
