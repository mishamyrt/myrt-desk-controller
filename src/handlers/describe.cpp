#include "mServer.h"

void registerDescribeHandler(mServer *server) {
  server->addRoute("/describe")
    .get([server]() {
      message["mac"] = WiFi.macAddress();
      message["rssi"] = WiFi.RSSI();
      message["id"] = String(ESP.getChipId(), HEX);
      server->send();
    });
}
