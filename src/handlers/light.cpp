#include "CommandHandler.h"
#include "Lightstrip.h"
#include "ArduinoJson.h"

void registerLightHandlers(Lightstrip *light) {
  Server.on("/light", [light]() {
    message.clear();
    message["brightness"] = light->brightness;
    message["enabled"] = light->enabled;
    JsonArray color = message.createNestedArray("color");
    color.add(light->r);
    color.add(light->g);
    color.add(light->b);
    sendJson();
  });

  Server.on("/light/power_on", [light]() {
    is_success = light->powerOn();
    message.clear();
    message["status"] = is_success ? "ok" : "error";
    sendJson();
  });

  Server.on("/light/power_off", [light]() {
    is_success = light->powerOff();
    message.clear();
    message["status"] = is_success ? "ok" : "error";
    sendJson();
  });

  Server.on("/light/set_color", [light]() {
    if (!Server.hasArg("plain")) {
      return sendError();
    }
    message.clear();
    DeserializationError error = deserializeJson(
      message,
      Server.arg("plain")
    );
    if (error) {
      return sendError();
    }
    is_success = light->setColor(
      message["brightness"],
      message["color"][0],
      message["color"][1],
      message["color"][2]
    );
    message.clear();
    message["status"] = is_success ? "ok" : "error";
    sendJson(200);
  });
}
