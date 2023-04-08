/* 
 * File:   crsf_uart.h
 * Author: bemcg
 *
 * Created on March 9, 2023, 2:57 PM
 */

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

