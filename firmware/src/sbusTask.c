
#include "definitions.h"
#include "sbusTask.h"
#include "sbus.h"
#include "debug.h"
#include "rtosHandles.h"

TaskHandle_t sbusTaskHandle;


void sbusTask(void *pvParameters) {
    initSBus();
    debugMsg("SBus starting");
    while (1) {
        vTaskDelay(7);
        sendSBusPacket();
        xTaskNotify(watchdogTaskHandle, SBUS_TASK_NOTIFY, eSetBits);
    }
}