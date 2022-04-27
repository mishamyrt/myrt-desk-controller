// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "helpers.h"

CRGB blend_colors(CRGB first, CRGB second, uint8_t ratio) {
  return CRGB(
    blend8(first.r, second.r, ratio),
    blend8(first.g, second.g, ratio),
    blend8(first.b, second.b, ratio)
  );
}
