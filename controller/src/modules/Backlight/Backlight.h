// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "lib/AVRLord/AVRLord.h"
#include "lib/DapMaster/DapMaster.h"
#include "BacklightController.h"

extern DapMaster LightData;
extern AVRLord LightAVR;
extern BacklightController Backlight;
extern FirmwareReader Reader;
