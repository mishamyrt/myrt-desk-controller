// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Arduino.h"
#include "WiFiClient.h"
#include <WiFi.h>
#include <AsyncUDP.h>
#include <ESPmDNS.h>

#include <secrets.h>
#include <pins.h>
#include <Blink.h>
#include <OTA.h>
#include <Loggr.h>
#include <Store.h>

// New domains
#include <DomainCommander.h>
#include <BacklightDomain.h>
#include <LegsDomain.h>
#include <SystemDomain.h>

// Server
AsyncUDP udp;
DomainCommander Commander;

const uint16_t port = 11011;

void handlePacket(AsyncUDPPacket packet) {
  Commander.handle(&packet);
}

void setupServer() {
  Loggr.setProtocol(&udp);
  // New UDP binary domains
  Commander.add(&BacklightDomain);
  Commander.add(&LegsDomain);
  Commander.add(&SystemDomain);
  if(udp.listen(port)) {
    udp.onPacket(handlePacket);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  blink(1);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  setupServer();
  OTA.initialize();
  blink(3);
}

void loop() {
  Height.handle();
  OTA.handle();
  Backlight.handle();
  Store.handle();
}
