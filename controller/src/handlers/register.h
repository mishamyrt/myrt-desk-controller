// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <Backlight.h>
#include <mServer.h>
#include <Height.h>
#include <Illuminance.h>

void registerDescribeHandler(mServer *server, String mac_address);
void registerLightstripHandlers(
  mServer *server,
  BacklightController *light,
  FirmwareReader *reader
);
void registerLegsHandlers(mServer *server, HeightController *height);
void registerSensorHandlers(mServer *server, IlluminanceSensor *illuminance);
