#include "definitions.h"
#include "tasks.h"
#include "rtosHandles.h"
#include "statusTask.h"
#include "sensorTask.h"
#include "packetTask.h"

QueueHandle_t debugQueue;

void initTasks(void) {
    xTaskCreate(statusTask, "statusTask", 128, NULL, 1, &statusTaskHandle);
    xTaskCreate(sensorTask, "sensorTask", 512, NULL, 2, &sensorTaskHandle);
    debugQueue = xQueueCreate(5, DEBUG_MAX_MSG_LEN);
    vTaskStartScheduler();
}
