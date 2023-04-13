/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: crsf_uart.h                               //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: rx uart runctions                  //
/////////////////////////////////////////////////////

#ifndef CRSF_UART_H
#define	CRSF_UART_H

#ifdef	__cplusplus
extern "C" {
#endif

void initUart(void);
void writeUart(uint8_t *buffer, int len);

#ifdef	__cplusplus
}
#endif

#endif	/* CRSF_UART_H */

