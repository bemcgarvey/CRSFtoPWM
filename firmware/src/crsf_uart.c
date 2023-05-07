/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: crsf_uart.c                               //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: rx uart runctions                  //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "crsf_uart.h"
#include "crsf.h"
#include "rtosHandles.h"
#include "passthrough.h"

volatile uint32_t lastByteTick = 0;

volatile static uint8_t buffer[64];
volatile static int pos = 0;
volatile static int remaining;
volatile static bool synched = false;

void T3Callback(TC_TIMER_STATUS status, uintptr_t context);

void initUart(void) {
    TC3_TimerCallbackRegister(T3Callback, (uintptr_t) NULL);
    lastByteTick = 0;
    synched = false;
    TC3_TimerStart();
    SERCOM0_REGS->USART_INT.SERCOM_INTENSET = (uint8_t) (SERCOM_USART_INT_INTENSET_ERROR_Msk | SERCOM_USART_INT_INTENSET_RXC_Msk);
}

void writeUart(uint8_t *buffer, int len) {
    if (!DMAC_ChannelIsBusy(DMAC_CHANNEL_0)) {
        DMAC_ChannelTransfer(DMAC_CHANNEL_0, buffer, (void *) &(SERCOM0_REGS->USART_INT.SERCOM_DATA), len);
    }
}

void rxISR(void) {
    //Handle rx
    uint8_t temp;
    temp = SERCOM0_REGS->USART_INT.SERCOM_DATA;
    if (passthroughEnabled) {
        xQueueSendToBackFromISR(passthroughQueueS0toS1, &temp, NULL);
        return;
    }
    if (!synched) {
        if (lastByteTick > 4) {
            synched = true;
            pos = 0;
        }
    }
    lastByteTick = 0;
    if (synched) {
        buffer[pos] = temp;
        if (pos == 1) {
            if (temp > 2 && temp <= MAX_CRSF_PACKET) {
                remaining = temp;
            } else {
                synched = false; //bad length so start over
                pos = 0;
            }
        } else if (pos > 1) {
            --remaining;
        }
        ++pos;
        if (pos > 1 && remaining == 0) {
            pos = 0;
            BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
            xQueueOverwriteFromISR(packetQueue, (void *) buffer, &pxHigherPriorityTaskWoken);
            portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
        }
    }
}

void SERCOM0_CRSF_USART_InterruptHandler(void) {
    uint8_t testCondition;
    testCondition = SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk;
    if (testCondition) {
        lastByteTick = 0;
        SERCOM0_USART_ErrorGet();
        synched = false;
        pos = 0;
    }
    testCondition = SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk;
    if (testCondition) {
        rxISR();
    }
}

void T3Callback(TC_TIMER_STATUS status, uintptr_t context) {
    ++lastByteTick;
}