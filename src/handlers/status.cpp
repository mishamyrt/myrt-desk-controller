#include "CommandHandler.h"
#include "ArduinoJson.h"

void registerStatusHandler() {
  Server.on("/", []() {
    message["mac"] = WiFi.macAddress();
    message["rssi"] = WiFi.RSSI();
    message["id"] = ESP.getChipId();
    sendJson();
  });
}
