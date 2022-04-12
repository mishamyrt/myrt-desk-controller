#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "OTAListener.h"

class OTAController {
  public:
    void initialize() {
      ArduinoOTA.setPort(6100);
      ArduinoOTA.setHostname("MyrtDesk");
      ArduinoOTA.begin();
      ArduinoOTA.onStart([this](){
        this->onStart();
      });
      // TODO: Add password (18+ symbols)
    }

    void setListener(OTAListener *listener) {
      this->listener = listener;
    }

    void onStart() {
      listener->onOTAStart();
    }

    void handle() {
      ArduinoOTA.handle();
    }
  private:
    OTAListener *listener;
};

extern OTAController OTA;
