#include "definitions.h"
#include "settings.h"

Settings settings;

__attribute__((aligned(NVMCTRL_FLASH_ROWSIZE), space(prog))) const union {
    Settings settings;
    uint8_t bytes[NVMCTRL_FLASH_ROWSIZE]; 
} romSettings = {{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 50, 0, 0, 1, 0, 0, 1.0}};

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

    return true;
}