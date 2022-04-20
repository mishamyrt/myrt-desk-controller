#include "Height.h"

Bekant Motors(PIN_BEKANT_UP, PIN_BEKANT_DOWN);
SonicMotion Motion(PIN_USONIC_TRIGGER, PIN_USONIC_ECHO);
HeightController Height(&Motors, &Motion);
