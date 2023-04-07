#include "definitions.h"
#include "rxTask.h"
#include "rtosHandles.h"
#include "crsf.h"
#include "debug.h"

TaskHandle_t rxTaskHandle;
static uint8_t packetBuffer[CHANNEL_PACKET_LEN];

void rxTask(void *pvParameters) {
    unsigned int packetCount = 0;
    debugMsg("Starting rx task");
    initCRSF();
    while (1) {
        if (xQueueReceive(packetQueue, packetBuffer, portMAX_DELAY) == pdTRUE) {
            ++packetCount;
            if (packetCount % 100 == 0) {
                debugMsg("*");
            }
        }
    }
}