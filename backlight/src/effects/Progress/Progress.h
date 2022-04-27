// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Effector.h>

class ProgressEffect : public Effect {
 public:
  bool handle(EffectStateController *host, Strip *strip);
  void finalize(EffectStateController *host);
  void onColorUpdate(EffectStateController *host);
  void prepare(EffectStateController *host, Strip *strip);

 private:
  uint8_t _stage;
  TimeProgress _progress = TimeProgress();
  TimeProgress _color_progress = TimeProgress();
  CRGB _background_color = CRGB::Black;
  CRGB _highlight_color;
  uint8_t _glow_side = 50;

  void _drawIntro(Strip *strip);
  void _drawOutro(EffectStateController *host, Strip *strip);
  void _drawProgress(Strip *strip);
  void _drawColorChange(EffectStateController *host, Strip *strip);
  void _drawGlow(Strip *strip);
};

extern ProgressEffect Progress;
