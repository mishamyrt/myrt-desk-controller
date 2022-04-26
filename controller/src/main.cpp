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
#include <Store.h>

#include <Height.h>
#include <Backlight.h>

#include "handlers/register.h"

// Server
AsyncWebServer AsyncServer(80);
AsyncWebSocket ws("/events");
mServer Server(&AsyncServer);

IlluminanceSensor Illuminance(PIN_PHOTORESISTOR, 100);

void setupServer() {
  Loggr.attach(&ws);
  AsyncServer.addHandler(&ws);
  registerDescribeHandler(&Server, WiFi.macAddress());
  registerLightstripHandlers(&Server, &Backlight, &Reader);
  registerLegsHandlers(&Server, &Height);
  registerSensorHandlers(&Server, &Illuminance);
  Server.initialize();
}

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.begin(115200);
  Backlight.connect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  setupServer();
  OTA.initialize();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  blink(3);
}

void loop() {
  OTA.handle();
  Height.handle();
  Backlight.handle();
  Store.handle();
}
