// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Illuminance.h"
#include "mServer.h"

void registerSensorHandlers(mServer *server, IlluminanceSensor *illuminance) {
  server->addRoute("/sensors")
    .get("/illuminance", [illuminance](AsyncWebServerRequest *request) {
      AsyncJsonResponse * response = new AsyncJsonResponse();
      JsonVariant& json = response->getRoot();
      json["value"] = illuminance->getValue();
      response->setLength();
      request->send(response);
    });
}
