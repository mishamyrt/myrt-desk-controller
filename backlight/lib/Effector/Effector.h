// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <stdint.h>
#include <FastLED.h>
#include "Strip.h"
#include "helpers.h"
#include "include/TimeProgress.h"

class EffectStateController {
 public:
  // Hooks
  virtual void onColorUpdated() = 0;
  virtual void onEffectFinished() = 0;
  // Property getters
  virtual uint32_t transition_time() = 0;
  virtual CRGB color() = 0;
  virtual CRGB target_color() = 0;
};

class Effect {
 public:
  virtual bool handle(EffectStateController *controller, Strip *strip) = 0;
  virtual void finalize(EffectStateController *controller) = 0;
  virtual void onColorUpdate(EffectStateController *controller) = 0;
  virtual bool setArguments(EffectStateController *controller, uint8_t *args, uint8_t length) = 0;
  virtual bool setData(EffectStateController *controller, uint8_t *args, uint8_t length) = 0;
  virtual void prepare(EffectStateController *controller, Strip *strip) = 0;
};

class Effector : public EffectStateController {
 public:
  explicit Effector(Strip *leds) {
    _leds = leds;
  }

  void handle();
  void applyEffect(Effect *effect);
  void setEffect(Effect *effect);
  bool setEffectArgs(uint8_t *args, uint8_t length);
  bool setEffectData(uint8_t *args, uint8_t length);
  void setColor(uint8_t r, uint8_t g, uint8_t b);
  void setBrightness(uint8_t brightness);
  void setTransition(uint32_t time);

  void onColorUpdated() override {
    _color = _target_color;
  }

  void onEffectFinished() override {
    _effect = _next_effect;
    _next_effect = NULL;
    _effect->prepare(this, _leds);
  }

  uint32_t transition_time() override {
    return _transition_time;
  }

  CRGB color() override {
    return _color;
  }

  CRGB target_color() override {
    return _target_color;
  }

 private:
  // Main color, other effects should use it
  CRGB _color = CRGB::Black;
  CRGB _target_color = CRGB::Black;
  uint8_t _brightness = 0;
  uint8_t _target_brightness = 0;
  TimeProgress _progress = TimeProgress();
  uint32_t _transition_time = 300;  // ms
  uint64_t _next_render;
  uint64_t _transition_start_time;
  Strip *_leds;
  Effect *_effect = NULL;
  Effect *_next_effect = NULL;

  void _transitBrightness();
};
