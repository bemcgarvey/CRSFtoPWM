#include "definitions.h"
#include "passthroughTask.h"
#include "stdio.h"

TaskHandle_t passthroughTaskHandle;
bool passthroughEnabled = false;
bool CLIecho = false;
char commandLine[64];

void handleSerialCommand(void);

void passthroughTask(void *pvParameters) {
    char rxByte;
    int pos = 0;
    char newline = '\n';
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
    } else {
        prompt = false;
    }
    if (prompt) {
        printf("# ");
    }
}

void ptErrorISR(void) {

}

void ptRxISR(void) {

}

void SERCOM1_Passthrough_InterruptHandler(void) {
    uint8_t testCondition;
    testCondition = SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk;
    if (testCondition) {
        ptErrorISR();
    }
    testCondition = SERCOM1_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk;
    if (testCondition) {
        ptRxISR();
    }
}