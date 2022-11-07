// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Progress.h"

enum {
  STAGE_INTRO,
  STAGE_PROGRESS,
  STAGE_COLOR_CHANGE,
  STAGE_OUTRO
};

bool ProgressEffect::handle(EffectStateController *host, Strip *strip) {
  _progress.tick();
  if (_stage != STAGE_PROGRESS) {
    _color_progress.tick();
  }
  switch (_stage) {
    case STAGE_INTRO:
      _drawIntro(strip);
      break;
    case STAGE_PROGRESS:
      _drawProgress(strip);
      break;
    case STAGE_COLOR_CHANGE:
      _drawColorChange(host, strip);
      break;
    case STAGE_OUTRO:
      _drawOutro(host, strip);
      break;
  }
  if (_progress.is_done()) {
    _progress.reset();
  }
  return true;
}

bool ProgressEffect::setArguments(EffectStateController *controller, uint8_t *args, uint8_t length) {
  return false;
}

bool ProgressEffect::setData(EffectStateController *controller, uint8_t *data, uint8_t length) {
  return false;
}

void ProgressEffect::finalize(EffectStateController *host) {
  _color_progress.setDuration(host->transition_time());
  _stage = STAGE_OUTRO;
}

void ProgressEffect::onColorUpdate(EffectStateController *host) {
  _color_progress.setDuration(host->transition_time());
  _stage = STAGE_COLOR_CHANGE;
}

void ProgressEffect::prepare(EffectStateController *host, Strip *strip) {
  _progress.setMaxProgress(strip->length);
  _progress.setDuration(host->transition_time() * 8);
  _color_progress.setDuration(host->transition_time());
  _highlight_color = host->target_color();
  _stage = STAGE_INTRO;
}

void ProgressEffect::_drawIntro(Strip *strip) {
  // Blend background from current color to black
  _background_color = blend_colors(
    _highlight_color,
    CRGB::Black,
    _color_progress.value()
  );
  _drawGlow(strip);
  if (_color_progress.is_done()) {
    _stage = STAGE_PROGRESS;
  }
}

void ProgressEffect::_drawOutro(EffectStateController *host, Strip *strip) {
  // Blend background from black color to current color
  _background_color = blend_colors(
    CRGB::Black,
    _highlight_color,
    _color_progress.value()
  );
  _drawGlow(strip);
  if (_color_progress.is_done()) {
    host->onEffectFinished();
  }
}

void ProgressEffect::_drawProgress(Strip *strip) {
  _drawGlow(strip);
}

void ProgressEffect::_drawColorChange(
  EffectStateController *host,
  Strip *strip
) {
  _highlight_color = blend_colors(
    host->color(),
    host->target_color(),
    _color_progress.value()
  );
  _drawGlow(strip);
  if (_color_progress.is_done()) {
    _stage = STAGE_PROGRESS;
    host->onColorUpdated();
  }
}

void ProgressEffect::_drawGlow(Strip *strip) {
  int progress = _progress.value();
  strip->set(_background_color);
  strip->set(_highlight_color, progress);
  CRGB color;
  for (int i = 1; i <= _glow_side; i++) {
    color = blend_colors(
      _background_color,
      _highlight_color,
      255 - map(i, 0, _glow_side, 0, 255)
    );
    strip->set(color, (progress + i) % strip->length);
    strip->set(color, i > progress
      ? strip->length - (i - progress)
      : progress - i
    );
  }
}

ProgressEffect Progress;
