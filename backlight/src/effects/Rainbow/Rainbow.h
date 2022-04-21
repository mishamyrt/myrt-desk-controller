#pragma once

#include <Effector.h>

class RainbowEffect : public Effect {
  public:
    bool handle(EffectStateController *host, Strip *strip);
    void finalize(EffectStateController *controller);
    void onColorUpdate(EffectStateController *controller);
    void prepare(EffectStateController *controller, Strip *strip);

  private:
    TimeProgress _progress = TimeProgress();
    CHSV _first_color = CHSV(0, 255, 255);
    CHSV _second_color = CHSV(0, 255, 255);
    CHSV _third_color = CHSV(0, 255, 255);

    uint8_t _hueShift(uint8_t shift);
};

extern RainbowEffect Rainbow;
