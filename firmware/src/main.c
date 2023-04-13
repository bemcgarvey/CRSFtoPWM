#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "tasks.h"
#include "settings.h"
#include "serial.h"

//TODO file headers

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
        printf("Watchdog timeout!!\r\n");
    }
    printf("Starting\r\n");
    loadSettings();
    printf("Settings loaded\r\n");
    printf("Servo rate %d Hz, Sensor rate %d Hz\r\n", settings.servoRate, settings.sensorRate);
    if (serialDetected) {
        printf("Serial detected\r\n");
        serialMain(); //Does not return;
    }
    initTasks(); //Starts scheduler, does not return
    while (true) {

    }
    return ( EXIT_FAILURE);
}
