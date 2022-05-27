// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "Arduino.h"
#include "WiFiClient.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"

#include <secrets.h>
#include <pins.h>
#include <Blink.h>
#include <OTA.h>
#include <mServer.h>
#include <Illuminance.h>
#include <Loggr.h>
#include <Store.h>

#include <Height.h>
#include <Backlight.h>

#include "handlers/register.h"

// Server
AsyncWebServer AsyncServer(80);
AsyncWebSocket ws("/events");
mServer Server(&AsyncServer);

IlluminanceSensor Illuminance(PIN_PHOTORESISTOR, 100);

// TaskHandle_t HeightTask;

void setupServer() {
  Loggr.attach(&ws);
  AsyncServer.addHandler(&ws);
  registerDescribeHandler(&Server, WiFi.macAddress());
  registerLightstripHandlers(&Server, &Backlight, &Reader);
  registerLegsHandlers(&Server, &Height);
  registerSensorHandlers(&Server, &Illuminance);
  Server.initialize();
  Loggr.start();
}

// void handleHeight( void * pvParameters ){
//   for(;;){
//     Height.handle();
//   }
// }

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial2.begin(115200);
  Backlight.connect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  setupServer();
  OTA.initialize();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  blink(3);
  Serial.println("Started on ");
  Serial.print(WiFi.localIP());
}

void loop() {
  Height.handle();
  OTA.handle();
  Backlight.handle();
  Store.handle();
}
