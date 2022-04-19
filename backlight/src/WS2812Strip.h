#pragma once

#include <Effector.h>

template <uint8_t DATA_PIN, uint8_t LENGTH>
class WS2812Strip : public Strip {
  public:
    WS2812Strip(const struct CRGB & correction) {
      this->leds = new struct CRGB[LENGTH];
      this->length = LENGTH;
      LEDS
        .addLeds<WS2811, DATA_PIN, GRB>(leds, length)
        .setCorrection(correction);
      LEDS.setBrightness(0);
    }
};
