// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Arduino.h"
#include "WiFiClient.h"
#include <WiFi.h>
#include <AsyncUDP.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"

#include <secrets.h>
#include <pins.h>
#include <Blink.h>
#include <OTA.h>
#include <mServer.h>
#include <Loggr.h>
#include <Store.h>
#include <Height.h>

// New domains
#include <DomainCommander.h>
#include <Backlight.h>

#include "handlers/register.h"

// Server
AsyncWebServer AsyncServer(80);
AsyncWebSocket ws("/events");
mServer Server(&AsyncServer);
AsyncUDP udp;
DomainCommander Commander;

const uint16_t port = 11011;

void handlePacket(AsyncUDPPacket packet) {
  Commander.handle(&packet);
}

void setupServer() {
  Loggr.attach(&ws);
  AsyncServer.addHandler(&ws);
  // Old REST handlers
  registerDescribeHandler(&Server, WiFi.macAddress());
  registerLegsHandlers(&Server, &Height);
  // New UDP binary domains
  registerBacklightDomain(&Commander);
  Server.initialize();
  Loggr.start();
  if(udp.listen(port)) {
    udp.onPacket(handlePacket);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  blink(1);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial2.begin(115200);
  Backlight.connect();
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
