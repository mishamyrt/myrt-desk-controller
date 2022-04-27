// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "ESPAsyncWebServer.h"
class Loggr_ {
 public:
  String message;

  void attach(AsyncWebSocket *stream) {
    _stream = stream;
  }

  void print(String content) {
    _send(content);
  }

  void flush() {
    _send(message);
    message = "";
  }

 private:
  AsyncWebSocket *_stream;

  void _send(String content) {
    _stream->textAll(("log: " + content).c_str());
  }
};

extern Loggr_ Loggr;
