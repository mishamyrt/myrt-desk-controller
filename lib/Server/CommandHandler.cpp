#include "CommandHandler.h"
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ArduinoJson.h"

StaticJsonDocument<200> message;
char response_buffer[200];
size_t response_length;
bool is_success;

void sendJson() {
  sendJson(200);
}

void sendJson(int code) {
  response_length = serializeJson(message, response_buffer);
  message.clear();
  Server.send(code, "application/json", response_buffer, response_length);
}

void sendError() {
  message["status"] = "error";
  sendJson(400);
}
