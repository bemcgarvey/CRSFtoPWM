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
        float alt = getAltitude();
        //Altitude in m
        (void)alt;
        //TODO send it
        vTaskDelay(delay);
        float v = getVBat();
        //Voltage in V
        (void)v;
        //TODO send it
    }
}
