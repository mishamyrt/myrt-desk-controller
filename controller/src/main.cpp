#include "Arduino.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"

#include <secrets.h>
#include <pins.h>
#include <Blink.h>
#include <AVRLord.h>
#include <OTA.h>
#include <mServer.h>
#include <Illuminance.h>
#include <Loggr.h>

#include <DapMaster.h>
#include <Backlight.h>
#include "handlers/register.h"
#include "Height/Height.h"

// Server
AsyncWebServer AsyncServer(80);
AsyncWebSocket ws("/events");
mServer Server(&AsyncServer);

// Async I/O devices
DapMaster LightData(&Serial);
AVRLord LightAVR(PIN_LIGHTSTRIP_RESET);
Backlight Light(&LightData, &LightAVR);

FirmwareReader Reader = FirmwareReader();
IlluminanceSensor Illuminance(PIN_PHOTORESISTOR, 100);

bool up_pressed;
bool down_pressed;

void setup() {
  Loggr.attach(&ws);
  AsyncServer.addHandler(&ws);
  // Setup wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setHostname("MyrtDesk");
  MDNS.begin("MyrtDesk");
  Serial.begin(115200);
  // Setup light controller
  Light.connect();
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
  registerLightstripHandlers(&Server, &Light, &Reader);
  registerLegsHandlers(&Server, &Height);
  registerSensorHandlers(&Server, &Illuminance);
  Server.initialize();
  blink(3);
  // Light.writePowerOff();
}

void loop() {
  OTA.handle();
  Height.handle();
  Light.handle();
}
