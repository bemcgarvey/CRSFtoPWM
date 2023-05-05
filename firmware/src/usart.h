//TODO header
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

