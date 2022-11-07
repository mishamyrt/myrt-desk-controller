// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Rainbow.h"

bool RainbowEffect::handle(EffectStateController *host, Strip *strip) {
  _progress.tick();
  _first_color.hue = _hueShift(0);
  _second_color.hue = _hueShift(60);
  _third_color.hue = _hueShift(120);

  fill_gradient(
    strip->leds,
    strip->length / 2,
    _first_color,
    _second_color,
    _third_color,
    FORWARD_HUES
  );
  strip->mirror();
  if (_progress.is_done()) {
    _progress.reset();
  }
  return true;
}

void RainbowEffect::prepare(EffectStateController *controller, Strip *strip) {
  _progress.setDuration(
    controller->transition_time() * 37
  );
}

bool RainbowEffect::setArguments(EffectStateController *controller, uint8_t *args, uint8_t length) {
  return false;
}

bool RainbowEffect::setData(EffectStateController *controller, uint8_t *data, uint8_t length) {
  return false;
}

uint8_t RainbowEffect::_hueShift(uint8_t shift) {
  return (_progress.value() + shift) % 255;
}

void RainbowEffect::finalize(EffectStateController *controller) {
  // TODO: Add effect out animation
  controller->onEffectFinished();
}

void RainbowEffect::onColorUpdate(EffectStateController *controller) {
  // Just proof new color for next effect. This effect ignores defined color
    controller->onColorUpdated();
}

RainbowEffect Rainbow;
