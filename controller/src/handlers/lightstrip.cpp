#include "Arduino.h"
#include "Lightstrip.h"
#include "mServer.h"
#include "FirmwareReader.h"

const char *field_brightness PROGMEM = "brightness";
const char *field_enabled PROGMEM = "enabled";
const char *field_color PROGMEM = "color";
const char *field_active PROGMEM = "active";
const char *field_temperature PROGMEM = "temperature";
const char *field_transition PROGMEM = "transition";

const char *template_lightstrip_state PROGMEM = \
  "{\"effect\":%d,\"brightness\":%d,\"temperature\":%d,\"enabled\":%s,\"color\":[%d,%d,%d]}";

void registerLightstripHandlers(mServer *server, Lightstrip *light, FirmwareReader *reader) {
  server->addRoute("/lightstrip")
    .get([light](AsyncWebServerRequest *request) {
      AsyncResponseStream *response = request->beginResponseStream(JSON_MIMETYPE);
      RGB color = light->color();
      response->printf(
        template_lightstrip_state,
        light->effect(),
        light->brightness(),
        light->temperature(),
        light->enabled() ? "true" : "false",
        color.r, color.g, color.b
      );
      response->setCode(200);
      request->send(response);
    })

    .put("/effects", [light, server](AsyncWebServerRequest *request, JsonVariant &json) {
      if (!json.containsKey(field_active)) {
        return server->sendStatus(request, REQUEST_BAD);
      }
      bool is_success = light->setEffect(json[field_active]);
      server->sendStatus(request, is_success ? REQUEST_SUCCESS : REQUEST_ERROR);
    })

    .put("/color", [server, light](AsyncWebServerRequest *request, JsonVariant &json) {
      if (!(json.containsKey(field_color)
         || json.containsKey(field_temperature)
         || json.containsKey(field_brightness))) {
        return server->sendStatus(request, REQUEST_BAD);
      }
      bool planned = true;
      if (json.containsKey(field_brightness)) {
        planned = planned && light->setBrightness(json[field_brightness]);
      }
      if (json.containsKey(field_color)) {
        planned = planned && light->setColor(
          json[field_color][0],
          json[field_color][1],
          json[field_color][2]
        );
      } else if (json.containsKey(field_temperature)) {
        planned = planned && light->setTemperature(
          json[field_temperature]
        );
      }
      server->sendStatus(request, planned ? REQUEST_SUCCESS : REQUEST_ERROR);
    })

    .put("/transition", [server, light](AsyncWebServerRequest *request, JsonVariant &json) {
      if (!json.containsKey(field_transition)) {
        return server->sendStatus(request, REQUEST_BAD);
      }
      bool planned = light->setTransition(json[field_transition]);
      server->sendStatus(request, planned ? REQUEST_SUCCESS : REQUEST_ERROR);
    })

    .put("/power", [server, light](AsyncWebServerRequest *request, JsonVariant &json) {
      if (!json.containsKey(field_enabled)) {
        return server->sendStatus(request, REQUEST_BAD);
      }
      if (json[field_enabled] == light->enabled()) {
        return server->sendStatus(request, REQUEST_SUCCESS);
      }
      bool is_success;
      if (json[field_enabled]) {
        is_success = light->powerOn();
      } else {
        is_success = light->powerOff();
      }
      server->sendStatus(request, is_success ? REQUEST_SUCCESS : REQUEST_ERROR);
    })

    .postRaw("/update-firmware", [server, light, reader](AsyncWebServerRequest *request) {
      light->updateFirmware(reader);
      server->sendStatus(request, REQUEST_SUCCESS);
    }, [reader](AsyncWebServerRequest *request, uint8_t *data,
      size_t len,
      size_t index,
      size_t total
    ) {
      if(!index){
        reader->create(total);
      }
      reader->appendChunk(data, len, index);
    });
}
