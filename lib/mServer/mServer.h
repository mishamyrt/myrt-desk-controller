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
    mServer(AsyncWebServer *server) {
      this->server = server;
    }

    void initialize() {
      server->begin();
    }

    void sendStatus(AsyncWebServerRequest *request, RequestStatus status) {
      DynamicJsonDocument doc(64);
      AsyncResponseStream *response = request->beginResponseStream(JSON_MIMETYPE);
      switch (status) {
        case REQUEST_SUCCESS:
          doc[field_description] = "Success";
          response->setCode(200);
          break;
        case REQUEST_BAD:
          doc[field_description] = "Wrong data passed";
          response->setCode(400);
          break;
        case REQUEST_ERROR:
          doc[field_description] = "Internal error";
          response->setCode(500);
          break;
      }
      response_length = serializeJson(doc, response_buffer);
      response->write(response_buffer, response_length);
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
