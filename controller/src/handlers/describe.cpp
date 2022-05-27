// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "mServer.h"

const char *template_system_state PROGMEM = \
  "{\"mac\":\"%s\",\"rssi\":%d,\"free_heap\":%d}";

void registerDescribeHandler(mServer *server, String mac_address) {
  server->addRoute("/describe")
    .get([mac_address](AsyncWebServerRequest *request) {
      AsyncResponseStream *response = request->beginResponseStream(
        JSON_MIMETYPE
      );
      response->printf(
        template_system_state,
        mac_address,
        WiFi.RSSI(),
        ESP.getFreeHeap()
      );
      response->setCode(200);
      request->send(response);
    });
}
