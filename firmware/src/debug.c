#include "definitions.h"
#include "debug.h"
#include <stdio.h>
#include "rtosHandles.h"


void debugMsg(char *msg) {
    //TODO disable if debug level is 0
    xQueueSendToBack(debugQueue, msg, 0);
}

