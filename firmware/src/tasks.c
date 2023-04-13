#include "definitions.h"
#include "tasks.h"
#include "rtosHandles.h"
#include "statusTask.h"
#include "sensorTask.h"
#include "packetTask.h"
#include "rxTask.h"
#include "debug.h"
#include "crsf.h"
#include "settings.h"
#include "sbusTask.h"
#include "watchdogTask.h"

bool wdtResetHappened;

QueueHandle_t debugQueue;
QueueHandle_t packetQueue;

void initTasks(void) {
    xTaskCreate(statusTask, "statusTask", 128, NULL, 1, &statusTaskHandle);
    if (!wdtResetHappened) {
        xTaskCreate(sensorTask, "sensorTask", 512, NULL, 2, &sensorTaskHandle);
    }
    xTaskCreate(rxTask, "rxTask", 512, NULL, 4, &rxTaskHandle);
    xTaskCreate(watchdogTask, "watchdogTask", 128, NULL, 1, &watchdogTaskHandle);
    if (settings.sBusEnabled) {
        xTaskCreate(sbusTask, "sbusTask", 512, NULL, 3, &sbusTaskHandle);
    }
    debugQueue = xQueueCreate(5, DEBUG_MAX_MSG_LEN);
    packetQueue = xQueueCreate(1, CHANNEL_PACKET_LEN);
    WDT_Enable();
    vTaskStartScheduler();
}
