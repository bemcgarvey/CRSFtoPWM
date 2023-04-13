/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: rxTask.h                                  //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: rx task handler                    //
/////////////////////////////////////////////////////

#ifndef RXTASK_H
#define	RXTASK_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_CHANNELS    16

    extern uint16_t channels[NUM_CHANNELS];
    extern unsigned int packetCount;
    
    void rxTask(void *pvParameters);

#ifdef	__cplusplus
}
#endif

#endif	/* RXTASK_H */

