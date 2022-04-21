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

uint8_t RainbowEffect::_hueShift(uint8_t shift) {
  return (_progress.value() + shift) % 255;
}

void RainbowEffect::finalize(EffectStateController *controller) {
  // TODO: Add effect out animation
  controller->onEffectFinished();
}

void RainbowEffect::onColorUpdate(EffectStateController *controller) {
  // Do nothing. This effect ignores defined color
}

RainbowEffect Rainbow;
