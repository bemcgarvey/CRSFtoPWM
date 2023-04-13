/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: debug.c                                   //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: debug message function             //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "debug.h"
#include <stdio.h>
#include "rtosHandles.h"
#include <stdarg.h>


void debugMsg(const char *msg, ...) {
    char str[DEBUG_MAX_MSG_LEN];
    va_list args;
    va_start(args, msg);
    vsnprintf(str, DEBUG_MAX_MSG_LEN, msg, args);
    va_end(args);
    xQueueSendToBack(debugQueue, str, 0);
}

