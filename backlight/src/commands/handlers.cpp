#include <CommandServer.h>
#include <Animator.h>
#include <Effects.h>

#define COMMAND_GET_SYNC 0xFF
#define COMMAND_SET_COLOR 1
#define COMMAND_SET_BRIGHTNESS 2
#define COMMAND_SET_WHITE_TEMPERATURE 3
#define COMMAND_SET_TRANSITION 4
#define COMMAND_SET_EFFECT 5

#define COMMAND_EFFECT_NONE 0
#define COMMAND_EFFECT_RAINBOW 1

bool handleSetColor(uint8_t *message, uint8_t length) {
  if (length < 4) {
    return false;
  }
  Animator.setColor(message[1], message[2], message[3]);
  return true;
}

bool handleSetBrightness(uint8_t *message, uint8_t length) {
  if (length < 2) {
    return false;
  }
  Animator.setBrightness(message[1]);
  return true;
}

// Temperature is set like progress between 2700K and 6500K.
// For example, if `temperature` argument is 130,
// then real temperature is 2700+((6500-2700)*(130/255))=4637K.
bool handleSetTemperature(uint8_t *message, uint8_t length) {
  if (length < 2) {
    return false;
  }
  // Lightstrip calibrated to be 6500 Kelvin on white.
  // Blends with orange color, that gives about 2700K.
  CRGB color = blend(0xFFFFFF, 0xFF872B, message[1]);
  Animator.setColor(color.r, color.g, color.b);
  return true;
}

bool handleSetTransition(uint8_t *message, uint8_t length) {
  if (length < 3) {
    return false;
  }
  Animator.setTransition((message[1] << 8) + message[2]);
  return true;
}

bool handleSetEffect(uint8_t *message, uint8_t length) {
  if (length < 2) {
    return false;
  }
  switch (message[1]) {
    // TODO: Use mapper function
    case COMMAND_EFFECT_NONE:
      Animator.setEffect(&Fill);
      break;
    case COMMAND_EFFECT_RAINBOW:
      Animator.setEffect(&Rainbow);
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
}
