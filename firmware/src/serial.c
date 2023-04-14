/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: serial.h                                  //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: serial interface functions         //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "serial.h"
#include "commands.h"
#include "settings.h"
#include "sensors.h"
#include "version.h"

static uint8_t serialBuffer[64];

bool detectSerial(void) {
    bool detected = false;
    //Setup PA25 (OUT 10) as input with pullup enabled
    PORT_REGS->GROUP[0].PORT_DIRCLR = (1U << 25);
    PORT_REGS->GROUP[0].PORT_OUTSET = (1U << 25);
    PORT_REGS->GROUP[0].PORT_PINCFG[25] = 0x6U;
    if (((PORT_REGS->GROUP[0].PORT_IN >> 25) & 1U) == 0) {
        detected = true;
    }
    PORT_REGS->GROUP[0].PORT_OUTCLR = (1U << 25);
    return detected;
}

void serialMain(void) {

    uint8_t cmd;
    while (1) {
        SERCOM1_USART_Read(&cmd, 1);
        switch(cmd) {
            case CMD_GET_VERSION:
                *(uint32_t *)serialBuffer = DEVICE_SIGNATURE;
                *(uint16_t *)&serialBuffer[4] = version;
                SERCOM1_USART_Write(serialBuffer, 6);
                break;
            case CMD_GET_SETTINGS:
                SERCOM1_USART_Write(&settings, sizeof(Settings));
                *(float *)serialBuffer = getVBat();
                SERCOM1_USART_Write(serialBuffer, sizeof(float));
                break;
            case CMD_SAVE_SETTINGS:
                SERCOM1_USART_Read(&settings, sizeof(Settings));
                saveSettings();
                memset(&settings, 0, sizeof(Settings));
                loadSettings();
                SERCOM1_USART_Write(&settings, sizeof(Settings));
                break;
        }
    }
}
