#include "sensors.h"
#include "ms5637.h"

void initSensors(void) {
    ms5637Init();
}

float getAltitude(void) {
    return 0;
}

float getVBat(void) {
    return 0;
}
