//TODO header
#include "definitions.h"
#include "usart.h"
#include "settings.h"
#include "passthroughTask.h"

#define BAUD_VALUE_420000           (56360UL)
#define BAUD_VALUE_115200           (63019UL)
#define BAUD_VALUE_9600             (65326UL)

void switchSERCOM1Baud(SERCOM1_Baud baud) {
    //disable
    SERCOM1_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;
    while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
    switch (baud) {
        case BAUD_420000:
            SERCOM1_REGS->USART_INT.SERCOM_BAUD = (uint16_t) SERCOM_USART_INT_BAUD_BAUD(BAUD_VALUE_420000);
            break;
        case BAUD_115200:
            SERCOM1_REGS->USART_INT.SERCOM_BAUD = (uint16_t) SERCOM_USART_INT_BAUD_BAUD(BAUD_VALUE_115200);
            break;
        case BAUD_9600:
            SERCOM1_REGS->USART_INT.SERCOM_BAUD = (uint16_t) SERCOM_USART_INT_BAUD_BAUD(BAUD_VALUE_9600);
            break;
    }
    while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
    //Enable
    SERCOM1_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;
    while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
}

void SERCOM1_USART_InterruptHandler(void) {
    uint8_t testCondition;
    testCondition = SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk;
    if (testCondition) {
        SERCOM1_USART_ErrorGet();
    }
    testCondition = SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk;
    if (testCondition) {
        if (settings.uartMode == UART_PASSTHROUGH) {
            passthroughISR();
        } else if (settings.uartMode == UART_GPS) {
            //gpsISR();
        }
    }
}