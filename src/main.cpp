#include "Arduino.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"
#include "secret.h"

#include "Bekant.h"
#include "CommandServer.h"

#define BUTTON_UP D6
#define BUTTON_DOWN D5

#define OEM_UP D1
#define OEM_DOWN D2

Bekant desk;

bool up_pressed;
bool down_pressed;
uint16_t height;

void setup() {
  Serial.begin(9600);
  desk.initialize(OEM_UP, OEM_DOWN);
  createCommandServer(&desk);
  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Buttons
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  // Built'in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(600);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    height = Serial.parseInt();
    Serial.println("Got value");
    Serial.println(height);
    desk.moveTo(height);
  }

  if (digitalRead(BUTTON_UP)) {
    up_pressed = true;
    desk.moveUp();
  } else if (up_pressed) {
    up_pressed = false;
    desk.stopMovement();
  }

  if (digitalRead(BUTTON_DOWN)) {
    down_pressed = true;
    desk.moveDown();
  } else if (down_pressed) {
    down_pressed = false;
    desk.stopMovement();
  }


  desk.handleAction();
  handleClient();
}
