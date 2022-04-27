#include "Effector.h"

void Effector::handle() {
  if (_effect == NULL) {
    return;
  }
  unsigned long now = millis();
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
