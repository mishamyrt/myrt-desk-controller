// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Height.h"

Bekant Motors(PIN_BEKANT_UP, PIN_BEKANT_DOWN);
SonicMotion Motion(PIN_USONIC_TRIGGER, PIN_USONIC_ECHO);
HeightController Height(&Motors, &Motion);
