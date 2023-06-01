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

typedef enum {BAUD_9600 = 0, BAUD_19200 = 1, BAUD_38400 = 2, BAUD_57600 = 3, BAUD_115200 = 4, BAUD_420000 = 5} SERCOM1_Baud; 
void switchSERCOM1Baud(SERCOM1_Baud baud);

#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

