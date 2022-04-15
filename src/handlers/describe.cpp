#include "mServer.h"
#include "Loggr.h"

void registerDescribeHandler(mServer *server) {
  server->addRoute("/describe")
    .get([](AsyncWebServerRequest *request) {
      AsyncJsonResponse * response = new AsyncJsonResponse();
      JsonVariant& json = response->getRoot();
      json["mac"] = WiFi.macAddress();
      json["rssi"] = WiFi.RSSI();
      json["id"] = String(ESP.getChipId(), HEX);
      json["free_heap"] = ESP.getFreeHeap();
      response->setLength();
      request->send(response);
    });
}
