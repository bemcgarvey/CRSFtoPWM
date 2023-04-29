/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: settings.c                                //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: settings definitions and functions //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "settings.h"

Settings settings __attribute__((aligned(4)));

__attribute__((aligned(NVMCTRL_FLASH_ROWSIZE), space(prog))) const union {
    Settings settings;
    uint8_t bytes[NVMCTRL_FLASH_ROWSIZE]; 
} romSettings = {{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 50, 0, 0, 2, 0, 2, 0, 0.0}};
//TODO change default UART mode back to zero

bool altimeterHealthy = false;
bool failsafeEnabled = true;

bool loadSettings(void) {
    uint8_t *p = (uint8_t *) & settings;
    for (int i = 0; i < sizeof (Settings); ++i) {
        *p = romSettings.bytes[i];
        ++p;
    }
    return true;
}

bool saveSettings(void) {
    NVMCTRL_RowErase((uint32_t)&romSettings);
    while (NVMCTRL_IsBusy());
    int bytesRemaining = sizeof(Settings);
    uint32_t *data = (uint32_t *)&settings;
    uint32_t address = (uint32_t)&romSettings;
    while (bytesRemaining > 0) {
        NVMCTRL_PageWrite(data, address);
        while (NVMCTRL_IsBusy());
        bytesRemaining -= NVMCTRL_FLASH_PAGESIZE;
        data += NVMCTRL_FLASH_PAGESIZE / 4;
        address += NVMCTRL_FLASH_PAGESIZE;
    }
    return true;
}