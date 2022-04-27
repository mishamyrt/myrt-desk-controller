// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include <Arduino.h>
#include <CommandServer.h>
#include <Effector.h>
#include <Effects.h>
#include "WS2812Strip.h"
#include "commands/handlers.h"

#define LED_DT 11
#define LED_COUNT 120
#define COLOR_CORRECTION 0xFF7A60

CommandServer Server = CommandServer();
WS2812Strip<LED_DT, LED_COUNT> LEDStrip = WS2812Strip<LED_DT, LED_COUNT>(
  COLOR_CORRECTION
);
Effector Animator = Effector(&LEDStrip);

void setup() {
  Serial.begin(115200);
  Server.begin(&Serial);
  Animator.applyEffect(&Fill);
  registerHandlers(&Server);
}

void loop() {
  Server.handle();
  Animator.handle();
}
