#include "mServer.h"
#include "Bekant.h"

const char *field_height = "height";

void registerLegsHandlers(mServer *server, BekantHeight *height) {
  server->addRoute("/legs")
    .get([server, height]() {
      message[field_height] = height->get();
      server->send();
    })
    .put([server, height]() {
      if (!server->parseMessage() || !message.containsKey(field_height)) {
        return server->sendStatus(REQUEST_BAD);
      }
      server->sendStatus(REQUEST_SUCCESS);
    })
    .post("/calibrate", [server, height]() {
      server->sendStatus(REQUEST_SUCCESS);
    });
}
