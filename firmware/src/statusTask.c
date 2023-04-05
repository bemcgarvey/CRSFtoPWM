#include "definitions.h"
#include "statusTask.h"
#include "rtosHandles.h"
#include <stdio.h>

TaskHandle_t statusTaskHandle;

void statusTask(void *pvParameters) {
    char msg[DEBUG_MAX_MSG_LEN];
    while (1) {
        LED_Toggle();
        if (xQueueReceive(debugQueue, msg, 1000) == pdTRUE) {
            msg[DEBUG_MAX_MSG_LEN - 1] = '\0';
            printf(msg);
            printf("\r\n");
        }
    }
}
