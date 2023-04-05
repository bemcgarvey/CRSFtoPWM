#include "definitions.h"
#include "sensorTask.h"
#include "sensors.h"
#include "settings.h"
#include "debug.h"

TaskHandle_t sensorTaskHandle;

char msg[30];

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
        float v = getVBat();
        (void)v;
        //sprintf(msg, "%.1f V", v);
        //debugMsg(msg);
        //TODO send it
    }
}
