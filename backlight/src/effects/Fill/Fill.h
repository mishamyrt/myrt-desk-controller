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
