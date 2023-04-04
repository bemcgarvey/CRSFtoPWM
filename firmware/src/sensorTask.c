#include "definitions.h"
#include "sensorTask.h"
#include "sensors.h"

TaskHandle_t sensorTaskHandle;

void sensorTask(void *pvParameter) {
    vTaskDelay(100);
    initSensors();
    while (1) {
        vTaskDelay(250);
        getAltitude();
        vTaskDelay(250);
        getVBat();
    }
}
