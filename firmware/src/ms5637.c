#include "definitions.h"
#include "ms5637.h"
#include "settings.h"

#define MS5637_ADDRESS  0b1110110

enum {
    CMD_RESET = 0x1e, CMD_PROM_READ = 0xa0
};

static uint8_t cmd;
static uint16_t calibration[8];

uint8_t crc4(uint16_t n_prom[]);

void ms5637Reset(void) {
    cmd = CMD_RESET;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
}

void ms5637ReadCalibration(void) {
    cmd = CMD_PROM_READ;
    uint8_t *p = (uint8_t *)calibration;
    for (int i = 0; i < 7; ++i) {
        SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
        while (SERCOM3_I2C_IsBusy());
        SERCOM3_I2C_Read(MS5637_ADDRESS, p, 2);
        while (SERCOM3_I2C_IsBusy());
        calibration[i] = (calibration[i] >> 8) | (calibration[i] << 8);
        p += 2;
        cmd += 2;
    }
//    for (int i = 0; i < 7; ++i) {
//        printf("%04x\r\n", calibration[i]);
//    }
}

void ms5637Init(void) {
    ms5637Reset();
    ms5637ReadCalibration();
    uint8_t crc = (calibration[0] & 0xf000) >> 12;
    if (crc == crc4(calibration)) {
        altimeterHealthy = true;
    } else {
        altimeterHealthy = false;
    }
}

uint8_t crc4(uint16_t n_prom[]) // n_prom defined as 8x unsigned int (n_prom[8])
{
    int cnt;                // simple counter
    uint16_t n_rem = 0; // crc reminder
    uint8_t n_bit;
    n_prom[0] = ((n_prom[0]) & 0x0FFF); // CRC byte is replaced by 0
    n_prom[7] = 0;                      // Subsidiary value, set to 0
    for (cnt = 0; cnt < 16; cnt++)      // operation is performed on bytes
    {                                   // choose LSB or MSB
      if (cnt % 2 == 1)
        n_rem ^= (uint16_t)((n_prom[cnt >> 1]) & 0x00FF);
      else
        n_rem ^= (uint16_t)(n_prom[cnt >> 1] >> 8);
      for (n_bit = 8; n_bit > 0; n_bit--) {
        if (n_rem & (0x8000))
          n_rem = (n_rem << 1) ^ 0x3000;
        else
          n_rem = (n_rem << 1);
      }
    }
    n_rem = ((n_rem >> 12) & 0x000F); // final 4-bit reminder is CRC code
    return (n_rem ^ 0x00);
}
