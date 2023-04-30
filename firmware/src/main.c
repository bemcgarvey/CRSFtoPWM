/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: main.c                                    //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: main function                      //
/////////////////////////////////////////////////////

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "tasks.h"
#include "settings.h"
#include "serial.h"

int main(void) {
    /* Initialize all modules */
    wdtResetHappened = false;
    if (PM_ResetCauseGet() == PM_RESET_CAUSE_WDT_RESET) {
        wdtResetHappened = true;
    }
    bool serialDetected = false;
    if (!wdtResetHappened) {
        serialDetected = detectSerial();
    }
    SYS_Initialize(NULL);
    if (wdtResetHappened) {
        if (settings.uartMode == UART_DEBUG) {
            printf("Watchdog timeout!!\r\n");
        }
    }
    if (settings.uartMode == UART_DEBUG) {
        printf("Starting\r\n");
    }
    loadSettings();
    if (settings.uartMode == UART_DEBUG) {
        printf("Settings loaded\r\n");
        printf("Servo rate %d Hz, Sensor rate %d Hz\r\n", settings.servoRate, settings.sensorRate);
    }
    if (serialDetected) {
        if (settings.uartMode == UART_DEBUG) {
            printf("Serial detected\r\n");
        }
        serialMain(); //Does not return;
    }
    initTasks(); //Starts scheduler, does not return
    while (true) {

    }
    return ( EXIT_FAILURE);
}
