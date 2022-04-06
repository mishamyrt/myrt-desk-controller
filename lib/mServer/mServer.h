#pragma once

#include "ESP8266WebServer.h"
#include "Client.h"
#include "ArduinoJson.h"
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
    mServer(ESP8266WebServer *esp_server) {
      server = esp_server;
    }

    void initialize() {
      server->begin();
    }

    void handle() {
      server->handleClient();
    }

    bool parseMessage() {
      if (!server->hasArg("plain")) {
        return false;
      }
      message.clear();
      DeserializationError error = deserializeJson(
        message,
        server->arg("plain")
      );
      if (error) {
        return false;
      }
      return true;
    }

    void sendStatus(RequestStatus status) {
      message.clear();
      switch (status) {
        case REQUEST_SUCCESS:
          message[field_description] = "Success";
          return send(200);
        case REQUEST_BAD:
          message[field_description] = "Wrong data passed";
          return send(400);
        case REQUEST_ERROR:
          message[field_description] = "Internal error";
          return send(500);
      }
    }

    void send(int code = 200) {
      response_length = serializeJson(message, response_buffer);
      message.clear();
      server->send(code, json_mime, response_buffer, response_length);
    }

    mRouter addRoute(String prefix) {
      return mRouter(server, prefix);
    }

  private:
    ESP8266WebServer *server;
    char response_buffer[200];
    size_t response_length;
};
