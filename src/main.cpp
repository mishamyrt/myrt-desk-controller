#include "Arduino.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

#include "secrets.h"
#include "pins.h"
#include "Blink.h"
#include "Bekant.h"
#include "Lightstrip.h"
#include "OTA.h"
#include "mServer.h"
#include "Illuminance.h"
#include "handlers.h"

// mServer
ESP8266WebServer ESPServer(80);
mServer Server(&ESPServer);

// I/O devices
BekantHeight Height(PIN_OEM_UP, PIN_OEM_DOWN);
Lightstrip Light(PIN_LIGHTSTRIP_RESET);
IlluminanceSensor Illuminance(PIN_PHOTORESISTOR, 100);

bool up_pressed;
bool down_pressed;

void setup() {
  // Setup light controller
  Light.initialize();
  // TODO: Replace with progress
  Light.setColor(10, 50, 0, 255);
  // Setup wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setHostname("MyrtDesk");
  MDNS.begin("MyrtDesk");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Light.setColor(30, 40, 255, 0);
  // Setup height controllers;
  Height.initialize();
  OTA.initialize();
  // Setup and disable LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Setup buttons for manual height adjustment
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);
  // Setup request handlers and start server
  registerDescribeHandler(&Server);
  registerLightstripHandlers(&Server, &Light);
  registerLegsHandlers(&Server, &Height);
  registerSensorHandlers(&Server, &Illuminance);
  Server.initialize();
  blink(3);
  Light.powerOff();
}

void loop() {
  if (digitalRead(PIN_BUTTON_UP)) {
    up_pressed = true;
    Height.increase();
  } else if (up_pressed) {
    up_pressed = false;
    Height.stop();
  }

  if (digitalRead(PIN_BUTTON_DOWN)) {
    down_pressed = true;
    Height.decrease();
  } else if (down_pressed) {
    down_pressed = false;
    Height.stop();
  }
  Height.handle();
  Server.handle();
  OTA.handle();
}
