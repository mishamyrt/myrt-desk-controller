// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "include/Flagpole.h"
#include "include/TimeProgress.h"

CRGB blend_colors(CRGB first, CRGB second, uint8_t ratio);
