/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: debug.h                                   //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: debug message function             //
/////////////////////////////////////////////////////

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DEBUG_MAX_MSG_LEN   30   
    
    void debugMsg(const char *msg, ...);


#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

