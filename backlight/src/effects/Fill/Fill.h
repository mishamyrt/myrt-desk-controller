// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Effector.h>

class FillEffect : public Effect {
 public:
  bool handle(EffectStateController *host, Strip *strip);
  void finalize(EffectStateController *controller);
  void onColorUpdate(EffectStateController *controller);
  void prepare(EffectStateController *controller, Strip *strip);

 private:
  bool _transitioned;
  TimeProgress _progress = TimeProgress();
};

extern FillEffect Fill;
