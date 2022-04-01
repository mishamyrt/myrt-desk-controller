#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "Bekant.h"

ESP8266WebServer http(80);
String message;
Bekant *controller;


String formatStatus() {
  message = "{\"is_ready\":";
  if (controller->isReady()) {
    message += "true";
  } else {
    message += "false";
  }
  message += ",\"height\":";
  message += String(controller->getHeight());
  message += "}";
  return message;
}

void handleRoot() {
  http.send(200, "application/json", formatStatus());
}

void handleHeight() {
  if (!http.hasArg("height")) {
    http.send(400, "application/json", "{\"error\": \"Height not provided\"}");
    return;
  }
  controller->moveTo(
    http.arg("height").toInt()
  );
  Serial.println(http.arg("height").toInt());
  http.send(200, "application/json", formatStatus());
}

void createCommandServer(Bekant *desk_controller) {
  controller = desk_controller;
  http.on("/", handleRoot);
  http.on("/update", handleHeight);
  http.begin();
}

void handleClient() {
  http.handleClient();
}
