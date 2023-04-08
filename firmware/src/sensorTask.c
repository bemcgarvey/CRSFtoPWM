#include "definitions.h"
#include "sensorTask.h"
#include "sensors.h"
#include "settings.h"
#include "debug.h"
#include "crsf.h"

TaskHandle_t sensorTaskHandle;

void sensorTask(void *pvParameter) {
    vTaskDelay(100);
    initSensors();
    int delay;
    float dAlt = 0.37;
    delay = 500 / settings.sensorRate;
    debugMsg("Sensors initialized");
    if (altimeterHealthy) {
        debugMsg("Altimeter good");
    } else {
        debugMsg("Altimeter failed");
    }
    while (1) {
        vTaskDelay(delay);
        float alt = getAltitude();
        //TODO need to smooth dAlt over at least a second
        //TODO need to calculate relative altitude 
        sendAltitudeTelem(alt, dAlt);
        vTaskDelay(delay);
        float v = getVBat();
        sendBatteryTelem(v);
    }
}
