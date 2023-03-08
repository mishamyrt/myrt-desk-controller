// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "GradientFill.h"

bool GradientFillEffect::handle(EffectStateController *host, Strip *strip) {
  if (_transitioned) {
    return false;
  }
  _progress.tick();
  CHSV blended_target = nblend(_color, _target_color, _progress.value());
  _secondary_color = blended_target;
  if (_secondary_color.hue <= GRADIENT_SHIFT) {
    _secondary_color.hue = 255 - (GRADIENT_SHIFT - _secondary_color.hue);
  } else {
    _secondary_color.hue = _secondary_color.hue - GRADIENT_SHIFT;
  }
  fill_gradient(
    strip->leds,
    strip->length / 2,
    _secondary_color,
    blended_target,
    FORWARD_HUES
  );
  strip->mirror();
  if (_progress.is_done()) {
    host->onColorUpdated();
    _transitioned = true;
  }
  return true;
}

void GradientFillEffect::finalize(EffectStateController *controller) {
  controller->onEffectFinished();
}

bool GradientFillEffect::setArguments(EffectStateController *controller, uint8_t *args, uint8_t length) {
  return false;
}

bool GradientFillEffect::setData(EffectStateController *controller, uint8_t *data, uint8_t length) {
  return false;
}

void GradientFillEffect::onColorUpdate(EffectStateController *controller) {
  _progress.setDuration(
    controller->transition_time()
  );
  _transitioned = false;
  _color = rgb2hsv_approximate(controller->color());
  _target_color = rgb2hsv_approximate(controller->target_color());
}

void GradientFillEffect::prepare(EffectStateController *controller, Strip *strip) {
  onColorUpdate(controller);
}

GradientFillEffect GradientFill;
