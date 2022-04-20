#include <pins.h>
#include "Backlight.h"

DapMaster LightData(&Serial);
AVRLord LightAVR(PIN_LIGHTSTRIP_RESET);
BacklightController Backlight(&LightData, &LightAVR);
FirmwareReader Reader = FirmwareReader();
