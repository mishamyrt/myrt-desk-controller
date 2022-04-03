#include "Arduino.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"

#include "secrets.h"
#include "Blink.h"
#include "Bekant.h"
#include "Lightstrip.h"
#include "CommandServer.h"
#include "OTA.h"
#include "./handlers/handlers.h"

#define BUTTON_UP D6
#define BUTTON_DOWN D5

#define OEM_UP D1
#define OEM_DOWN D2

bool up_pressed;
bool down_pressed;

BekantHeight Height;
Lightstrip Light;

void setup() {
  // Setup wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setHostname("MyrtDesk");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // Setup connection with light and height controllers;
  Height.initialize(OEM_UP, OEM_DOWN);
  Light.initialize(115200, D0);
  Light.setColor(128, 255, 0, 0);
  OTA.initialize();
  // Setup and disable LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Setup buttons for manual height adjustment
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  // Setup request handlers and start server
  registerStatusHandler();
  registerLightHandlers(&Light);
  Server.begin();
  blink(3);
}

void loop() {

  if (digitalRead(BUTTON_UP)) {
    up_pressed = true;
    Height.increase();
  } else if (up_pressed) {
    up_pressed = false;
    Height.stop();
  }

  if (digitalRead(BUTTON_DOWN)) {
    down_pressed = true;
    Height.decrease();
  } else if (down_pressed) {
    down_pressed = false;
    Height.stop();
  }
  Height.handle();
  Server.handleClient();
  OTA.handle();
}
