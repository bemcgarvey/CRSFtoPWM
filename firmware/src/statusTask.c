#include "definitions.h"
#include "debug.h"
#include "statusTask.h"
#include "rtosHandles.h"
#include <stdio.h>

TaskHandle_t statusTaskHandle;

void statusTask(void *pvParameters) {
    char msg[DEBUG_MAX_MSG_LEN];
    printf("Starting status task\r\n");
    while (1) {
        if (xQueueReceive(debugQueue, msg, 1000) == pdTRUE) {
            msg[DEBUG_MAX_MSG_LEN - 1] = '\0';
            printf(msg);
            printf("\r\n");
            LED_Toggle();
        }
    }
}
