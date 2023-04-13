/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: statusTask.c                              //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: status task handler                //
/////////////////////////////////////////////////////

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
        if (xQueueReceive(debugQueue, msg, portMAX_DELAY) == pdTRUE) {
            msg[DEBUG_MAX_MSG_LEN - 1] = '\0';
            printf(msg);
            printf("\r\n");
            LED_Toggle();
        }
    }
}
