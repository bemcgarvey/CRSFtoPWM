#include "definitions.h"
#include "statusTask.h"
#include <stdio.h>

TaskHandle_t statusTaskHandle;

void statusTask(void *pvParameters) {
    int count = 0;
    while (1) {
        ++count;
        printf("Count = %d\r\n", count);
        LED_Toggle();
        vTaskDelay(1000);
    }
}
