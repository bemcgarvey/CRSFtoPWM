/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: usart.h                                   //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: SERCOM1 usart ISR                  //
/////////////////////////////////////////////////////

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {BAUD_9600, BAUD_115200, BAUD_420000} SERCOM1_Baud; 
void switchSERCOM1Baud(SERCOM1_Baud baud);

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

