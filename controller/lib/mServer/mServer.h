// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include "mRouter.h"
#include "./include/strings.h"

enum RequestStatus {
  REQUEST_SUCCESS,
  REQUEST_BAD,
  REQUEST_ERROR
};

extern StaticJsonDocument<200> message;

/// Wrappper around ESP8266WebServer for convenience
class mServer {
 public:
  explicit mServer(AsyncWebServer *server) {
    this->server = server;
  }

  void initialize() {
    server->begin();
  }

  void sendStatus(AsyncWebServerRequest *request, RequestStatus status) {
    AsyncResponseStream *response = request->beginResponseStream(JSON_MIMETYPE);
    response->print("{\"description\":\"");
    switch (status) {
      case REQUEST_SUCCESS:
        response->print("Success");
        response->setCode(200);
        break;
      case REQUEST_BAD:
        response->print("Wrong data passed");
        response->setCode(400);
        break;
      case REQUEST_ERROR:
        response->print("Internal error");
        response->setCode(500);
        break;
    }
    response->print("\"}");
    request->send(response);
  }

  mRouter addRoute(String prefix) {
    return mRouter(server, prefix);
  }

 private:
  AsyncWebServer *server;
  char response_buffer[200];
  size_t response_length;
};
