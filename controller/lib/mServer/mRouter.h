#pragma once

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include "Client.h"
#include "./include/strings.h"

class mRouter {
  public:
    mRouter(AsyncWebServer *server, String uri_prefix = root) {
      this->server = server;
      prefix = uri_prefix;
    }

    void done() {
      delete this;
    }

    mRouter& get(ArRequestHandlerFunction fn) {
      return this->get(root, fn);
    }

    mRouter& get(String uri, ArRequestHandlerFunction fn) {
      return this->on(uri, HTTP_GET, fn);
    }

    mRouter& post(ArJsonRequestHandlerFunction fn) {
      return this->post(root, fn);
    }

    mRouter& post(String uri, ArJsonRequestHandlerFunction fn) {
      return this->onJson(uri, HTTP_POST, fn);
    }

    mRouter& put(ArJsonRequestHandlerFunction fn) {
      return this->put(root, fn);
    }

    mRouter& put(String uri, ArJsonRequestHandlerFunction fn) {
      return this->onJson(uri, HTTP_PUT, fn);
    }

    mRouter& postRaw(String uri, ArRequestHandlerFunction fn, ArBodyHandlerFunction body_fn) {
      AsyncCallbackWebHandler* handler = createHanlder(uri, HTTP_POST, fn);
      handler->onBody(body_fn);
      server->addHandler(handler);
      return *this;
    }

    mRouter& on(String uri, WebRequestMethodComposite method, ArRequestHandlerFunction fn) {
      server->addHandler(
        createHanlder(uri, method, fn)
      );
      return *this;
    }

    mRouter& onJson(String uri, WebRequestMethodComposite method, ArJsonRequestHandlerFunction fn) {
      server->addHandler(
        createJsonHanlder(uri, method, fn)
      );
      return *this;
    }

  private:
    String prefixed (String uri) {
      if (uri == "/") {
        return prefix;
      } else {
        return prefix + uri;
      }
    }

    AsyncCallbackWebHandler* createHanlder(String uri, WebRequestMethodComposite method, ArRequestHandlerFunction fn) {
      AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
      handler->setMethod(method);
      handler->onRequest(fn);
      handler->setUri(prefixed(uri));
      return handler;
    }

    AsyncCallbackJsonWebHandler* createJsonHanlder(String uri, WebRequestMethodComposite method, ArJsonRequestHandlerFunction fn) {
      AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler(prefixed(uri), fn);
      handler->setMethod(method);
      return handler;
    }

    AsyncWebServer* server;
    String prefix;
};
