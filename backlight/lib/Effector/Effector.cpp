// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Effector.h"

void Effector::handle() {
  if (_effect == NULL) {
    return;
  }
  uint64_t now = millis();
  if (now < _next_render) {
    return;
  }
  bool should_update = false;
  if (_brightness != _target_brightness) {
    _transitBrightness();
    should_update |= true;
  }
  should_update |= _effect->handle(this, _leds);
  if (should_update) {
    LEDS.show();
    _next_render = now + 30;
  }
}

void Effector::applyEffect(Effect *effect) {
  _effect = effect;
  _effect->prepare(this, _leds);
  LEDS.show();
}

void Effector::setEffect(Effect *effect) {
  _next_effect = effect;
  _effect->finalize(this);
}

bool Effector::setEffectArgs(uint8_t *args, uint8_t length) {
  return _effect->setArguments(this, args, length);
}

bool Effector::setEffectData(uint8_t *args, uint8_t length) {
  return _effect->setData(this, args, length);
}

void Effector::setColor(uint8_t r, uint8_t g, uint8_t b) {
  _target_color.r = r;
  _target_color.g = g;
  _target_color.b = b;
  _effect->onColorUpdate(this);
}

void Effector::setBrightness(uint8_t brightness) {
  _target_brightness = brightness;
  _progress.setDuration(_transition_time);
}

void Effector::setTransition(uint32_t time) {
  _transition_time = time;
}

void Effector::_transitBrightness() {
  _progress.tick();
  LEDS.setBrightness(
    blend8(_brightness, _target_brightness, _progress.value())
  );
  if (_progress.is_done()) {
    _brightness = _target_brightness;
  }
}
