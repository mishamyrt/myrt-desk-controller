#include "CommandHandler.h"
#include "Bekant.h"
#include "ArduinoJson.h"

void registerHeightHandlers(BekantHeight *height) {
  Server.on("/height/set", [height]() {
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
    height->set(message["height"]);
    message.clear();
    message["status"] = "ok";
    sendJson();
  });

  Server.on("/height", [height]() {
    message.clear();
    message["height"] = height->get();
    sendJson();
  });
}
