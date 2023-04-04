#include "definitions.h"
#include "statusTask.h"

TaskHandle_t statusTaskHandle;

void statusTask(void *pvParameters) {
    while (1) {
        LED_Toggle();
        vTaskDelay(1000);
    }
}
