#include "definitions.h"
#include "tasks.h"
#include "rtosHandles.h"
#include "statusTask.h"

void initTasks(void) {
    xTaskCreate(statusTask, "statusTask", 128, NULL, 1, &statusTaskHandle);
    vTaskStartScheduler();
}
