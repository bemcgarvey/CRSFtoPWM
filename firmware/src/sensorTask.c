#include "definitions.h"
#include "sensorTask.h"
#include "sensors.h"
#include "settings.h"
#include "debug.h"

TaskHandle_t sensorTaskHandle;

void sensorTask(void *pvParameter) {
    vTaskDelay(100);
    initSensors();
    int delay;
    delay = 500 / settings.sensorRate;
    debugMsg("Sensors initialized");
    if (altimeterHealthy) {
        debugMsg("Altimeter good");
    } else {
        debugMsg("Altimeter failed");
    }
    while (1) {
        vTaskDelay(delay);
        getAltitude();
        //TODO send it
        vTaskDelay(delay);
        getVBat();
        //TODO send it
    }
}
