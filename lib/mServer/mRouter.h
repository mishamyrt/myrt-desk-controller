#pragma once

#include "ESP8266WebServer.h"
#include "Client.h"
#include "Lightstrip.h"

extern Lightstrip Light;

class mRouter {
  public:
    mRouter(ESP8266WebServer *esp_server, String uri_prefix) {
      server = esp_server;
      prefix = uri_prefix;
    }

    void done() {
      delete this;
    }

    mRouter& post(std::function<void ()> fn) {
      return this->on("/", HTTP_POST, fn);
    }

    mRouter& post(String uri, std::function<void ()> fn) {
      return this->on(uri, HTTP_POST, fn);
    }

    mRouter& get(std::function<void ()> fn) {
      return this->on("/", HTTP_GET, fn);
    }

    mRouter& get(String uri, std::function<void ()> fn) {
      return this->on(uri, HTTP_GET, fn);
    }

    mRouter& put(std::function<void ()> fn) {
      return this->on("/", HTTP_PUT, fn);
    }

    mRouter& put(String uri, std::function<void ()> fn) {
      return this->on(uri, HTTP_PUT, fn);
    }

    mRouter& on(String uri, HTTPMethod method, std::function<void ()> fn) {
      if (uri == "/") {
        server->on(prefix, method, fn);
      } else {
        server->on(prefix + uri, method, fn);
      }
      return *this;
    }

  ESP8266WebServer *server;
  String prefix;
};
