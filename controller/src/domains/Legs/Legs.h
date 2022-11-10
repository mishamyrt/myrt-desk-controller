// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <pins.h>
#include <DomainCommander.h>
#include "Controller/HeightController.h"
#include "Bekant/Bekant.h"

extern Bekant Motors;
extern SensorReader HeightReader;
extern HeightController Height;

extern Domain LegsDomain;
