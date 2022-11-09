// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Effector.h>

struct Zone {
  uint8_t start = 0;
  uint8_t end = 0;
  CRGB last_color = CRGB::Black;
  CRGB target_color = CRGB::Black;
};

class AmbientEffect : public Effect {
 public:
  bool handle(EffectStateController *host, Strip *strip);
  void finalize(EffectStateController *controller);
  void onColorUpdate(EffectStateController *controller);
  void prepare(EffectStateController *controller, Strip *strip);
  bool setArguments(EffectStateController *controller, uint8_t *args, uint8_t length);
  bool setData(EffectStateController *controller, uint8_t *data, uint8_t length);

 private:
  bool _transitioned;
  Zone _zones[32];
  uint8_t _zones_count = 0;
  TimeProgress _progress = TimeProgress();

  void _fillZone(uint8_t idx, CRGB color, Strip *strip);
};

extern AmbientEffect Ambient;
