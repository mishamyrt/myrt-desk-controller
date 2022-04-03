#pragma once

#include "Arduino.h"
#include "ESP8266WebServer.h"
#include "ArduinoJson.h"

extern ESP8266WebServer Server;
extern StaticJsonDocument<200> message;
extern char response_buffer[200];
extern size_t response_length;
extern bool is_success;

void sendJson(int code);
void sendJson();
void sendError();
