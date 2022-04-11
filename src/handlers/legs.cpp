#include "mServer.h"
#include "Bekant.h"

const char *field_height PROGMEM = "height";

void registerLegsHandlers(mServer *server, BekantHeight *height) {
  server->addRoute("/legs")
    .get([height](AsyncWebServerRequest *request) {
      AsyncJsonResponse * response = new AsyncJsonResponse();
      JsonVariant& json = response->getRoot();
      json[field_height] = height->get();
      response->setLength();
      request->send(response);
    })
    .put([height, server](AsyncWebServerRequest *request, JsonVariant &json) {
      if (!json.containsKey(field_height)) {
        return server->sendStatus(request, REQUEST_BAD);
      }
      height->set(json[field_height]);
      server->sendStatus(request, REQUEST_SUCCESS);
    });
    // .post("/calibrate", [server, height]() {
    //   server->sendStatus(REQUEST_SUCCESS);
    // });
}
