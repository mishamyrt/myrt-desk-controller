#include "Illuminance.h"
#include "mServer.h"

void registerSensorHandlers(mServer *server, IlluminanceSensor *illuminance) {
  server->addRoute("/sensors")
    .get("/illuminance", [server, illuminance]() {
      message["value"] = illuminance->getValue();
      server->send();
    });
}
