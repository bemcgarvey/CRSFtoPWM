#include "definitions.h"
#include "passthroughTask.h"
#include "stdio.h"
#include "rtosHandles.h"
#include "crsf.h"
#include "crsf_uart.h"

TaskHandle_t passthroughTaskHandle;
QueueHandle_t passthroughQueue;
volatile bool passthroughEnabled = false;

static bool CLIecho = false;
static char commandLine[64];

void handleSerialCommand(void);
void switchSERCOM1Baud(bool fast);

void passthroughTask(void *pvParameters) {
    char rxByte;
    int pos = 0;
    char newline = '\n';
    NVIC_SetPriority(SERCOM1_IRQn, 1);
    while (1) {
        while (!SERCOM1_USART_Read(&rxByte, 1));
        if (CLIecho && rxByte != '\n') {
            SERCOM1_USART_Write(&rxByte, 1);
        }
        if (rxByte == '\r') {
            if (pos != 0) {
                SERCOM1_USART_Write(&newline, 1);
                commandLine[pos] = '\0';
                handleSerialCommand();
                pos = 0;
            }
        } else if (rxByte == '\n') {
            pos = 0;
        } else {
            commandLine[pos] = rxByte;
            ++pos;
            if (pos >= 64) {
                pos = 0;
            }
        }
    }
}

void handleSerialCommand(void) {
    bool prompt = true;
    if (strncmp(commandLine, "#", 1) == 0) {
        vTaskSuspend(sensorTaskHandle);
        printf("CLI Mode, type 'exit' or 'help'\r\n");
        CLIecho = true;
    } else if (strncmp(commandLine, "serial", 64) == 0) {
        printf("serial 0 64 0 0 0 0\r\n");
    } else if (strncmp(commandLine, "get serialrx_provider", 64) == 0) {
        printf("serialrx_provider = CRSF\r\n");
    } else if (strncmp(commandLine, "get serialrx_inverted", 64) == 0) {
        printf("serialrx_inverted = OFF\r\n");
    } else if (strncmp(commandLine, "get serialrx_halfduplex", 64) == 0) {
        printf("serialrx_halfduplex = OFF\r\n");
    } else if (strncmp(commandLine, "serialpassthrough 0 ", 20) == 0) {
        LED_Set();
        passthroughEnabled = true;
        WDT_Disable();
        vTaskSuspend(watchdogTaskHandle);
        vTaskSuspend(rxTaskHandle);
        while (DMAC_ChannelIsBusy(DMAC_CHANNEL_0));
        switchSERCOM1Baud(true);
        SERCOM1_REGS->USART_INT.SERCOM_INTENSET = (uint8_t) (SERCOM_USART_INT_INTENSET_ERROR_Msk | SERCOM_USART_INT_INTENSET_RXC_Msk);
        NVIC_EnableIRQ(SERCOM1_IRQn);
        lastByteTick = 0;
        while (passthroughEnabled) {
            vTaskDelay(1000);
            if (lastByteTick >= 6000) {
                passthroughEnabled = false;
            }
        }
        NVIC_DisableIRQ(SERCOM1_IRQn);
        SERCOM1_REGS->USART_INT.SERCOM_INTENCLR = (uint8_t) (SERCOM_USART_INT_INTENSET_ERROR_Msk | SERCOM_USART_INT_INTENSET_RXC_Msk);
        switchSERCOM1Baud(false);
        vTaskResume(watchdogTaskHandle);
        vTaskResume(rxTaskHandle);
        vTaskResume(sensorTaskHandle);
        WDT_Enable();
        LED_Clear();
    } else {
        prompt = false;
    }
    if (prompt) {
        printf("# ");
    }
}

void SERCOM1_Passthrough_InterruptHandler(void) {
    uint8_t testCondition;
    uint8_t temp;
    testCondition = SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk;
    if (testCondition) {
        USART_ERROR errorStatus = USART_ERROR_NONE;
        errorStatus = (USART_ERROR) (SERCOM1_REGS->USART_INT.SERCOM_STATUS & (uint16_t) (SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk));
        if (errorStatus != USART_ERROR_NONE) {
            /* Clear error flag */
            SERCOM1_REGS->USART_INT.SERCOM_INTFLAG = (uint8_t) SERCOM_USART_INT_INTFLAG_ERROR_Msk;
            /* Clear all errors */
            SERCOM1_REGS->USART_INT.SERCOM_STATUS = (uint16_t) (SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk);
            /* Flush existing error bytes from the RX FIFO */
            while ((SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & (uint8_t) SERCOM_USART_INT_INTFLAG_RXC_Msk) == (uint8_t) SERCOM_USART_INT_INTFLAG_RXC_Msk) {
                temp = (uint8_t) SERCOM1_REGS->USART_INT.SERCOM_DATA;
            }
            (void) temp;
        }
    }
    testCondition = SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk;
    if (testCondition) {
        temp = SERCOM1_REGS->USART_INT.SERCOM_DATA;
        while ((SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & (uint8_t) SERCOM_USART_INT_INTFLAG_DRE_Msk) == 0U);
        SERCOM0_REGS->USART_INT.SERCOM_DATA = temp;
        lastByteTick = 0;
    }
}

#define SERCOM0_USART_INT_BAUD_VALUE            (56360UL)
#define SERCOM1_USART_INT_BAUD_VALUE            (63019UL)

void switchSERCOM1Baud(bool fast) {
    //disable
    SERCOM1_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;
    while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
    if (fast) {
        //set to 420000 baud
        SERCOM1_REGS->USART_INT.SERCOM_BAUD = (uint16_t) SERCOM_USART_INT_BAUD_BAUD(SERCOM0_USART_INT_BAUD_VALUE);
        while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
    } else {
        //set to 115200 baud
        SERCOM1_REGS->USART_INT.SERCOM_BAUD = (uint16_t) SERCOM_USART_INT_BAUD_BAUD(SERCOM1_USART_INT_BAUD_VALUE);
        while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
    }
    //Enable
    SERCOM1_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;
    while ((SERCOM1_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U);
}