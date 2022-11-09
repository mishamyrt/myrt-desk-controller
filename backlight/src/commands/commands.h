// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#define COMMAND_GET_SYNC 0xFF
#define COMMAND_SET_COLOR 1
#define COMMAND_SET_BRIGHTNESS 2
#define COMMAND_SET_WHITE_TEMPERATURE 3
#define COMMAND_SET_TRANSITION 4
#define COMMAND_SET_EFFECT 5
#define COMMAND_SET_EFFECT_DATA 6

#define COMMAND_EFFECT_NONE 0
#define COMMAND_EFFECT_RAINBOW 1
#define COMMAND_EFFECT_PROGRESS 2
#define COMMAND_EFFECT_AMBIENT 3
