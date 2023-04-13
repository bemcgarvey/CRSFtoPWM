/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: serial.h                                  //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: serial interface functions         //
/////////////////////////////////////////////////////

#ifndef SERIAL_H
#define	SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

    bool detectSerial(void);
    void serialMain(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

