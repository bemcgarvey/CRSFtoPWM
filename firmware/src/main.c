#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "tasks.h"
#include "settings.h"

//TODO file headers

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    printf("Starting\r\n");
    loadSettings();
    printf("Settings loaded\r\n");
    printf("%d Hz %d Hz %.1fV\r\n", settings.servoRate, settings.sensorRate, settings.batCalibration);
    initTasks();  //Starts scheduler, does not return
    while ( true )
    {
        
    }
    return ( EXIT_FAILURE );
}
