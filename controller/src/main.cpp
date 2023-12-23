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
#include <Store.h>

#include <AsyncSocketServer.h>
#include <DomainCommander.h>
#include <BacklightDomain.h>
#include <LegsDomain.h>
#include <SystemDomain.h>

// Server
AsyncUDP udp;
SocketServer server;
DomainCommander commander;

String hostname = "MyrtDesk";
const uint16_t port = 11011;

void setupServer() {
  // Setup domains
  commander.add(&BacklightDomain);
  commander.add(&LegsDomain);
  commander.add(&SystemDomain);

  // Start server
  if(udp.listen(port)) {
    server.attach(&udp);
    server.setHandler(&commander);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Store.initialize();
  blink(1);
  WiFi.disconnect(true, true);
  delay(500);
  WiFi.setHostname(hostname.c_str());
  WiFi.mode(WIFI_STA);
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
