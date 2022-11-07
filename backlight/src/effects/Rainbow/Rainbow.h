// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Effector.h>

class RainbowEffect : public Effect {
 public:
  bool handle(EffectStateController *host, Strip *strip);
  void finalize(EffectStateController *controller);
  void onColorUpdate(EffectStateController *controller);
  void prepare(EffectStateController *controller, Strip *strip);
  bool setArguments(EffectStateController *controller, uint8_t *args, uint8_t length);
  bool setData(EffectStateController *controller, uint8_t *data, uint8_t length);

 private:
  TimeProgress _progress = TimeProgress();
  CHSV _first_color = CHSV(0, 255, 255);
  CHSV _second_color = CHSV(0, 255, 255);
  CHSV _third_color = CHSV(0, 255, 255);

  uint8_t _hueShift(uint8_t shift);
};

extern RainbowEffect Rainbow;
