#include "Arduino.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"

#include <secrets.h>
#include <pins.h>
#include <Blink.h>
#include <OTA.h>
#include <mServer.h>
#include <Illuminance.h>
#include <Loggr.h>

#include <Height.h>
#include <Backlight.h>

#include "handlers/register.h"


// Server
AsyncWebServer AsyncServer(80);
AsyncWebSocket ws("/events");
mServer Server(&AsyncServer);

IlluminanceSensor Illuminance(PIN_PHOTORESISTOR, 100);

bool up_pressed;
bool down_pressed;

void setup() {
  // Early enable WS server
  Loggr.attach(&ws);
  AsyncServer.addHandler(&ws);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.begin(115200);
  // Setup light controller
  Backlight.connect();
  // TODO: Replace with progress
  // Light.setColor(10, 50, 0, 255);
  // Light.handle();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // Light.setColor(30, 40, 255, 0);
  // Light.handle();
  // Setup height controllers;
  // Height.initialize();
  OTA.initialize();
  // Setup and disable LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Setup request handlers and start server
  registerDescribeHandler(&Server, WiFi.macAddress());
  registerLightstripHandlers(&Server, &Backlight, &Reader);
  registerLegsHandlers(&Server, &Height);
  registerSensorHandlers(&Server, &Illuminance);
  Server.initialize();
  blink(3);
}

void loop() {
  OTA.handle();
  Height.handle();
  Backlight.handle();
}
