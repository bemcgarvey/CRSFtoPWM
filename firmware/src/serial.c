
#include "definitions.h"
#include "serial.h"
#include "commands.h"
#include "settings.h"
#include "sensors.h"
#include "version.h"

static uint8_t serialBuffer[64];

bool detectSerial(void) {
    bool detected = false;
    //Setup PA25 (CH10) as input with pullup enabled
    PORT_REGS->GROUP[0].PORT_DIR = 0;
    PORT_REGS->GROUP[0].PORT_PINCFG[25] = 0x6U;
    for (int i = 0; i < 100; ++i);  //short delay
    if (((PORT_REGS->GROUP[0].PORT_IN >> 25) & 1U) == 0) {
        detected = true;
    }
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
                break;
            case CMD_GET_VOLTAGE:
                *(float *)serialBuffer = getVBat();
                SERCOM1_USART_Write(serialBuffer, sizeof(float));
                break;
            case CMD_SAVE_SETTINGS:
                //TODO do we need to preserve the battery calibration value?
                SERCOM1_USART_Read(&settings, sizeof(Settings));
                saveSettings();
                memset(&settings, 0, sizeof(Settings));
                loadSettings();
                SERCOM1_USART_Write(&settings, sizeof(Settings));
                break;
            case CMD_SET_VOLTAGE:
                //TODO can this just be done when saving settings?
                SERCOM1_USART_Read(serialBuffer, sizeof(float));
                settings.batCalibration = *(float *)serialBuffer - getVBat();
                saveSettings();
                break;
        }
    }
}
