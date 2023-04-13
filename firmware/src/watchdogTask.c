/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: watchdogTask.c                            //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: watchdog task handler              //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "watchdogTask.h"
#include "settings.h"
#include "rtosHandles.h"
#include "debug.h"

TaskHandle_t watchdogTaskHandle;

void watchdogTask(void *pvParameters) {
    uint32_t activeMask;
    debugMsg("Watchdog task starting");
    activeMask = RX_TASK_NOTIFY;
    if (settings.sBusEnabled) {
        activeMask |= SBUS_TASK_NOTIFY;
    }
    while (1) {
        vTaskDelay(50);
        uint32_t statusFlags;
        xTaskNotifyWait(0, 0xffffffff, &statusFlags, 0);
        if ((statusFlags & activeMask) == activeMask) {
            WDT_Clear();
        }
    }
    
}
