/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: version.c                                 //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: firmware version definition        //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "version.h"

#define MAJOR_VERSION   0
#define MINOR_VERSION   9

uint16_t version = (MAJOR_VERSION << 8) | MINOR_VERSION;
