#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "tasks.h"

//TODO file headers

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    printf("Starting\r\n");
    initTasks();  //Starts scheduler, does not return
    while ( true )
    {
        
    }
    return ( EXIT_FAILURE );
}
