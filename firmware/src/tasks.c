#include "definitions.h"
#include "tasks.h"
#include "rtosHandles.h"
#include "statusTask.h"
#include "sensorTask.h"
#include "packetTask.h"

void initTasks(void) {
    xTaskCreate(statusTask, "statusTask", 128, NULL, 1, &statusTaskHandle);
    xTaskCreate(sensorTask, "sensorTask", 512, NULL, 2, &sensorTaskHandle);
    vTaskStartScheduler();
}
