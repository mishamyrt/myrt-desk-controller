// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Ambient.h"

bool AmbientEffect::handle(EffectStateController *host, Strip *strip) {
  if (_transitioned) {
    return false;
  }
  _progress.tick();
  CRGB color;
  size_t progress_value = _progress.value();
  for (uint8_t i = 0; i < _zones_count; i++) {
    color = blend_colors(
      _zones[i].last_color,
      _zones[i].target_color,
      progress_value
    );
    _fillZone(i, color, strip);
  }
  if (_progress.is_done()) {
    _transitioned = true;
  }
  return true;
}

void AmbientEffect::_fillZone(uint8_t idx, CRGB color, Strip *strip) {
  for (uint8_t i = _zones[idx].start; i < _zones[idx].end; i++) {
    strip->set(color, i);
  }
}

void AmbientEffect::finalize(EffectStateController *controller) {
  controller->onEffectFinished();
}

bool AmbientEffect::setArguments(EffectStateController *controller, uint8_t *args, uint8_t length) {
  if (length != (args[0] * 2) + 1) {
    return false;
  }
  // 3, 0, 33, 33, 66, 66, 99
  _zones_count = 0;
  for (uint8_t i = 1; i < length; i += 2) {
    _zones[_zones_count].start = args[i];
    _zones[_zones_count].end = args[i+1];
    _zones_count++;
  }
  return true;
}

bool AmbientEffect::setData(EffectStateController *controller, uint8_t *data, uint8_t length) {
  if (length != _zones_count * 3) {
    return false;
  }
  size_t progress_value = _progress.value();
  uint8_t offset = 0;
  for (uint8_t i = 0; i < _zones_count; i++) {
    _zones[i].last_color = blend_colors(
      _zones[i].last_color,
      _zones[i].target_color,
      progress_value
    );
    offset = i * 3;
    _zones[i].target_color.r = data[offset];
    _zones[i].target_color.g = data[offset + 1];
    _zones[i].target_color.b = data[offset + 2];
  }
  _transitioned = false;
  _progress.reset();
  return true;
}

void AmbientEffect::onColorUpdate(EffectStateController *controller) {
  return;
}

void AmbientEffect::prepare(EffectStateController *controller, Strip *strip) {
  _progress.setDuration(5000);
}

AmbientEffect Ambient;
