#include "definitions.h"
#include "settings.h"

Settings settings;
const uint8_t romSettings[NVMCTRL_FLASH_ROWSIZE] __attribute__((aligned(NVMCTRL_FLASH_ROWSIZE), keep, externally_visible, space(prog)))
    = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 0, 0, 0, 2, 0, 0};

bool altimeterHealthy = false;
bool failsafeEnabled = true;

bool loadSettings(void) {
    uint8_t *p = (uint8_t *) & settings;
    for (int i = 0; i < sizeof (Settings); ++i) {
        *p = romSettings[i];
        ++p;
    }
    //TODO add a crc check
    return true;
}

bool saveSettings(void) {

    return true;
}