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
    if (l == 1) {
      return Backlight.setEffect(m[0]);
    } else if (l > 1) {
      return Backlight.setEffect(m[0], &m[1], l - 1);
    }
    return false;
  });
  domain->on(COMMAND_BACKLIGHT_SET_EFFECT_DATA, [](uint8_t *m, size_t l, CommanderResponse *r) {
    return Backlight.setEffectData(m, l);
  });
  domain->on(
    COMMAND_BACKLIGHT_FIRMWARE_RECEIVE,
    [](uint8_t *m, size_t l, CommanderResponse *r) {
      if (l != 2) {
        return false;
      }
      Reader.create((m[0] << 8) + m[1]);
      return true;
    });
  domain->on(
    COMMAND_BACKLIGHT_FIRMWARE_FRAME,
    [](uint8_t *m, size_t l, CommanderResponse *r) {
      if (Reader.size() == 0 || m[l - 1] != 111) {
        return false;
      }
      return Reader.appendChunk(m, l - 1);
    });
  domain->on(
    COMMAND_BACKLIGHT_FIRMWARE_APPLY,
    [](uint8_t *m, size_t l, CommanderResponse *r) {
      if (Reader.size() == 0 || l != 0) {
        return false;
      }
      Backlight.updateFirmware(&Reader);
      return true;
    });
});

