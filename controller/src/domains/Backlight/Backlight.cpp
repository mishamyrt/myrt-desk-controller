#include "Backlight.h"
#include <pins.h>

DapMaster LightData(&Serial2);
AVRLord LightAVR(PIN_LIGHTSTRIP_RESET);
BacklightController Backlight(&LightData, &LightAVR);
FirmwareReader Reader = FirmwareReader();

void broadcastBacklightState(SocketResponse *r) {
  RGB color = Backlight.color();
  r->append(Backlight.enabled() ? 1 : 0);
  r->append(Backlight.effect());
  r->append(Backlight.mode());
  r->append(color.r);
  r->append(color.g);
  r->append(color.b);
  r->append(Backlight.temperature());
  r->append(Backlight.brightness());
  r->broadcast();
}

Domain BacklightDomain(DOMAIN_BACKLIGHT, [](Domain *domain) {
  Serial2.begin(115200);
  Backlight.connect();
  domain->on(COMMAND_BACKLIGHT_READ, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l > 0) {
      return false;
    }
    broadcastBacklightState(r);
    return true;
  });
  domain->on(COMMAND_BACKLIGHT_SET_COLOR, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l != 3) {
      return false;
    }
    bool success = Backlight.setColor(m[0], m[1], m[2]);
    if (success) {
      broadcastBacklightState(r);
    }
    return success;
  });
  domain->on(COMMAND_BACKLIGHT_SET_BRIGHTNESS, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l != 1) {
      return false;
    }
    bool success = Backlight.setBrightness(m[0]);
    if (success) {
      broadcastBacklightState(r);
    }
    return success;
  });
  domain->on(COMMAND_BACKLIGHT_SET_WHITE, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l != 1) {
      return false;
    }
    bool success = Backlight.setTemperature(m[0]);
    if (success) {
      broadcastBacklightState(r);
    }
    return success;
  });
  domain->on(COMMAND_BACKLIGHT_SET_POWER, [](uint8_t *m, size_t l, SocketResponse *r) {
    if (l != 1) {
      return false;
    }
    bool success = false;
    if (m[0] == 0) {
      success = Backlight.powerOff();
    } else if (m[0] == 1) {
      success = Backlight.powerOn();
    }
    if (success) {
      broadcastBacklightState(r);
    }
    return success;
  });

  domain->on(COMMAND_BACKLIGHT_SET_EFFECT, [](uint8_t *m, size_t l, SocketResponse *r) {
    bool success = false;
    if (l == 1) {
      success = Backlight.setEffect(m[0]);
    } else if (l > 1) {
      success = Backlight.setEffect(m[0], &m[1], l - 1);
    }
    if (success) {
      broadcastBacklightState(r);
    }
    return success;
  });
  domain->on(COMMAND_BACKLIGHT_SET_EFFECT_DATA, [](uint8_t *m, size_t l, SocketResponse *r) {
    return Backlight.setEffectData(m, l);
  });
  domain->on(
    COMMAND_BACKLIGHT_FIRMWARE_RECEIVE,
    [](uint8_t *m, size_t l, SocketResponse *r) {
      if (l != 2) {
        return false;
      }
      Reader.create((m[0] << 8) + m[1]);
      return true;
    });
  domain->on(
    COMMAND_BACKLIGHT_FIRMWARE_FRAME,
    [](uint8_t *m, size_t l, SocketResponse *r) {
      if (Reader.size() == 0 || m[l - 1] != 111) {
        return false;
      }
      return Reader.appendChunk(m, l - 1);
    });
  domain->on(
    COMMAND_BACKLIGHT_FIRMWARE_APPLY,
    [](uint8_t *m, size_t l, SocketResponse *r) {
      if (Reader.size() == 0 || l != 0) {
        return false;
      }
      Backlight.updateFirmware(&Reader);
      return true;
    });
});

