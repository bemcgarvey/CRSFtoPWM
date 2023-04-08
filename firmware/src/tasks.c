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

QueueHandle_t debugQueue;
QueueHandle_t packetQueue;

void initTasks(void) {
    xTaskCreate(statusTask, "statusTask", 128, NULL, 1, &statusTaskHandle);
    xTaskCreate(sensorTask, "sensorTask", 512, NULL, 2, &sensorTaskHandle);
    xTaskCreate(rxTask, "rxTask", 512, NULL, 4, &rxTaskHandle);
    if (settings.sBusEnabled) {
        xTaskCreate(sbusTask, "sbusTask", 512, NULL, 3, &sbusTaskHandle);
    }
    debugQueue = xQueueCreate(5, DEBUG_MAX_MSG_LEN);
    packetQueue = xQueueCreate(1, CHANNEL_PACKET_LEN);
    vTaskStartScheduler();
}
