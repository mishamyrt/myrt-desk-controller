#pragma once

#include <Backlight.h>
#include <mServer.h>
#include <Height.h>
#include <Illuminance.h>

void registerDescribeHandler(mServer *server, String mac_address);
void registerLightstripHandlers(mServer *server, BacklightController *light, FirmwareReader *reader);
void registerLegsHandlers(mServer *server, HeightController *height);
void registerSensorHandlers(mServer *server, IlluminanceSensor *illuminance);
