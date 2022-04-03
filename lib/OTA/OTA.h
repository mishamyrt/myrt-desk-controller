#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class OTAController {
  public:
    void initialize() {
      ArduinoOTA.setPort(6100);
      ArduinoOTA.setHostname("MyrtDesk");
      ArduinoOTA.begin();
      // TODO: Add password (18+ symbols)
    }

    void handle() {
      ArduinoOTA.handle();
    }
};

extern OTAController OTA;
