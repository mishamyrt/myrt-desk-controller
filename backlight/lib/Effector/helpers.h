#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "include/Flagpole.h"
#include "include/TimeProgress.h"

CRGB blend_colors(CRGB first, CRGB second, uint8_t ratio);
