// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#include "handlers.h"
#include <Animator.h>
#include <Effects.h>
#include <commands.h>

bool handleSetColor(uint8_t *message, uint8_t length) {
  if (length != 3) {
    return false;
  }
  Animator.setColor(message[0], message[1], message[2]);
  return true;
}

bool handleSetBrightness(uint8_t *message, uint8_t length) {
  if (length != 1) {
    return false;
  }
  Animator.setBrightness(message[0]);
  return true;
}

// Temperature is set like progress between 2700K and 6500K.
// For example, if `temperature` argument is 130,
// then real temperature is 2700+((6500-2700)*(130/255))=4637K.
bool handleSetTemperature(uint8_t *message, uint8_t length) {
  if (length != 1) {
    return false;
  }
  // Lightstrip calibrated to be 6500 Kelvin on white.
  // Blends with orange color, that gives about 2700K.
  CRGB color = blend(0xFFFFFF, 0xFF872B, message[0]);
  Animator.setColor(color.r, color.g, color.b);
  return true;
}

bool handleSetTransition(uint8_t *message, uint8_t length) {
  if (length != 2) {
    return false;
  }
  Animator.setTransition((message[0] << 8) + message[1]);
  return true;
}

bool handleSetEffectData(uint8_t *message, uint8_t length) {
  return Animator.setEffectData(message, length);
}

bool handleSetEffect(uint8_t *message, uint8_t length) {
  if (length < 1) {
    return false;
  }
  switch (message[0]) {
    case COMMAND_EFFECT_NONE:
      Animator.setEffect(&Fill);
      break;
    case COMMAND_EFFECT_RAINBOW:
      Animator.setEffect(&Rainbow);
      break;
    case COMMAND_EFFECT_PROGRESS:
      Animator.setEffect(&Progress);
      break;
    case COMMAND_EFFECT_AMBIENT:
      Animator.setEffect(&Ambient);
      if (length > 1) {
        return Animator.setEffectArgs(&message[1], length - 1);
      }
      break;
    default:
      return false;
  }
  return true;
}

void registerHandlers(CommandServer *server) {
  server->on(COMMAND_SET_COLOR, &handleSetColor);
  server->on(COMMAND_SET_BRIGHTNESS, &handleSetBrightness);
  server->on(COMMAND_SET_WHITE_TEMPERATURE, &handleSetTemperature);
  server->on(COMMAND_SET_TRANSITION, &handleSetTransition);
  server->on(COMMAND_SET_EFFECT, &handleSetEffect);
  server->on(COMMAND_SET_EFFECT_DATA, &handleSetEffectData);
}
