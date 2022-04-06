#pragma once

#include "Lightstrip.h"
#include "mServer.h"
#include "Bekant.h"
#include "Illuminance.h"

void registerDescribeHandler(mServer *server);
void registerLightstripHandlers(mServer *server, Lightstrip *light);
void registerLegsHandlers(mServer *server, BekantHeight *height);
void registerSensorHandlers(mServer *server, IlluminanceSensor *illuminance);
