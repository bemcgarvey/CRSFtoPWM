/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: passthrough.h                             //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: BetaFlight passthrough functions   //
/////////////////////////////////////////////////////

#ifndef PASSTHROUGH_H
#define	PASSTHROUGH_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern volatile bool passthroughEnabled;
    void passthroughTask(void *pvParameters);
    void passthroughISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PASSTHROUGH_H */

