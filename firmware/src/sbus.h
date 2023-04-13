/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: sbus.h                                    //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: sBus functions and definitions     //
/////////////////////////////////////////////////////

#ifndef SBUS_H
#define	SBUS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void initSBus(void);
    void sendSBusPacket(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SBUS_H */

