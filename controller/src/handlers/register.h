#pragma once

#include <Backlight.h>
#include <mServer.h>
#include <FirmwareReader.h>
#include <Bekant.h>
#include <Illuminance.h>

void registerDescribeHandler(mServer *server, String mac_address);
void registerLightstripHandlers(mServer *server, Backlight *light, FirmwareReader *reader);
void registerLegsHandlers(mServer *server, BekantHeight *height);
void registerSensorHandlers(mServer *server, IlluminanceSensor *illuminance);
