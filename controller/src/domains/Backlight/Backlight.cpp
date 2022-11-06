#include "Backlight.h"
#include <pins.h>

DapMaster LightData(&Serial2);
AVRLord LightAVR(PIN_LIGHTSTRIP_RESET);
BacklightController Backlight(&LightData, &LightAVR);
FirmwareReader Reader = FirmwareReader();

Domain BacklightDomain(DOMAIN_BACKLIGHT, [](Domain *domain) {
  Serial2.begin(115200);
  Backlight.connect();
  domain->on(COMMAND_BACKLIGHT_READ, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l > 0) {
      return false;
    }
    RGB color = Backlight.color();
    r->append(Backlight.enabled() ? 1 : 0);
    r->append(Backlight.effect());
    r->append(Backlight.mode());
    r->append(color.r);
    r->append(color.g);
    r->append(color.b);
    r->append(Backlight.temperature());
    r->append(Backlight.brightness());
    r->flush();
    return true;
  });
  domain->on(COMMAND_BACKLIGHT_SET_COLOR, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l != 3) {
      return false;
    }
    return Backlight.setColor(m[0], m[1], m[2]);
  });
  domain->on(COMMAND_BACKLIGHT_SET_BRIGHTNESS, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l != 1) {
      return false;
    }
    return Backlight.setBrightness(m[0]);
  });
  domain->on(COMMAND_BACKLIGHT_SET_WHITE, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l != 1) {
      return false;
    }
    return Backlight.setTemperature(m[0]);
  });
  domain->on(COMMAND_BACKLIGHT_TURN_OFF, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l != 0) {
      return false;
    }
    return Backlight.powerOff();
  });
  domain->on(COMMAND_BACKLIGHT_SET_EFFECT, [](uint8_t *m, size_t l, CommanderResponse *r) {
    if (l != 1) {
      return false;
    }
    return Backlight.setEffect(m[0]);
  });
});

