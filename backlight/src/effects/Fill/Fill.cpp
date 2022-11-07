// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Fill.h"

bool FillEffect::handle(EffectStateController *host, Strip *strip) {
  if (_transitioned) {
    return false;
  }
  _progress.tick();
  CRGB color = blend_colors(
    host->color(),
    host->target_color(),
    _progress.value()
  );
  strip->set(color);
  if (_progress.is_done()) {
    host->onColorUpdated();
    _transitioned = true;
  }
  return true;
}

void FillEffect::finalize(EffectStateController *controller) {
  controller->onEffectFinished();
}

bool FillEffect::setArguments(EffectStateController *controller, uint8_t *args, uint8_t length) {
  return false;
}

bool FillEffect::setData(EffectStateController *controller, uint8_t *data, uint8_t length) {
  return false;
}

void FillEffect::onColorUpdate(EffectStateController *controller) {
  _progress.setDuration(
    controller->transition_time()
  );
  _transitioned = false;
}

void FillEffect::prepare(EffectStateController *controller, Strip *strip) {
  onColorUpdate(controller);
}

FillEffect Fill;
