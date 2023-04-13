/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: sbusTask.c                                //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: sBus task handler                  //
/////////////////////////////////////////////////////

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