// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

typedef enum {
  MODE_RGB = 0,
  MODE_TEMPERATURE = 1
} ColorMode;

#define DOMAIN_BACKLIGHT 1
#define COMMAND_BACKLIGHT_READ 0
#define COMMAND_BACKLIGHT_SET_EFFECT 1
#define COMMAND_BACKLIGHT_SET_EFFECT_DATA 2
#define COMMAND_BACKLIGHT_SET_COLOR 3
#define COMMAND_BACKLIGHT_SET_WHITE 4
#define COMMAND_BACKLIGHT_SET_BRIGHTNESS 5
#define COMMAND_BACKLIGHT_TURN_OFF 6
#define COMMAND_BACKLIGHT_FIRMWARE_RECEIVE 7
#define COMMAND_BACKLIGHT_FIRMWARE_FRAME 8
#define COMMAND_BACKLIGHT_FIRMWARE_APPLY 9
