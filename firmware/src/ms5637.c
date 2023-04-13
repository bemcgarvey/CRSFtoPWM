/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: ms5637.c                                  //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: ms5637 sensor functions            //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "ms5637.h"
#include "settings.h"

#define MS5637_ADDRESS  0b1110110

enum {
    CMD_RESET = 0x1e,
    CMD_D1_256 = 0x40,
    CMD_D1_512 = 0x42,
    CMD_D1_1024 = 0x44,
    CMD_D1_2048 = 0x46,
    CMD_D1_4096 = 0x48,
    CMD_D1_8192 = 0x4a,
    CMD_D2_256 = 0x50,
    CMD_D2_512 = 0x52,
    CMD_D2_1024 = 0x54,
    CMD_D2_2048 = 0x56,
    CMD_D2_4096 = 0x58,
    CMD_D2_8192 = 0x5a,
    CMD_ADC_READ = 0x00,
    CMD_PROM_READ = 0xa0
};

static uint16_t C[8];

uint8_t crc4(uint16_t n_prom[]);

void ms5637Reset(void) {
    uint8_t cmd = CMD_RESET;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
}

void ms5637ReadCalibration(void) {
    uint8_t cmd = CMD_PROM_READ;
    uint8_t *p = (uint8_t *) C;
    for (int i = 0; i < 7; ++i) {
        SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
        while (SERCOM3_I2C_IsBusy());
        SERCOM3_I2C_Read(MS5637_ADDRESS, p, 2);
        while (SERCOM3_I2C_IsBusy());
        C[i] = (C[i] >> 8) | (C[i] << 8);
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
    uint8_t crc = (C[0] & 0xf000) >> 12;
    if (crc == crc4(C)) {
        altimeterHealthy = true;
    } else {
        altimeterHealthy = false;
    }
}

int32_t getPressure(void) {
    uint8_t buff[3];
    uint8_t cmd = CMD_D2_8192;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
    vTaskDelay(20);
    cmd = CMD_ADC_READ;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
    SERCOM3_I2C_Read(MS5637_ADDRESS, buff, 3);
    while (SERCOM3_I2C_IsBusy());
    uint32_t D2;
    D2 = buff[0];
    D2 <<= 8;
    D2 |= buff[1];
    D2 <<= 8;
    D2 |= buff[2];
    int64_t dT;
    dT = D2 - ((uint32_t)C[5] << 8);
    //int32_t temp;
    //temp = (dT * C[6]) >> 23;
    //temp += 2000;
    cmd = CMD_D1_8192;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
    vTaskDelay(20);
    cmd = CMD_ADC_READ;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
    SERCOM3_I2C_Read(MS5637_ADDRESS, buff, 3);
    while (SERCOM3_I2C_IsBusy());
    uint32_t D1;
    D1 = buff[0];
    D1 <<= 8;
    D1 |= buff[1];
    D1 <<= 8;
    D1 |= buff[2];
    int64_t off;
    off = ((uint64_t)C[2] << 17) + ((C[4] * dT) >> 6);
    int64_t sens;
    sens = ((uint64_t)C[1] << 16) + ((C[3] * dT) >> 7);
    int32_t p;
    p = (((D1 * sens) >> 21) - off) >> 15;
    return p;
}

uint8_t crc4(uint16_t n_prom[]) // n_prom defined as 8x unsigned int (n_prom[8])
{
    int cnt; // simple counter
    uint16_t n_rem = 0; // crc reminder
    uint8_t n_bit;
    n_prom[0] = ((n_prom[0]) & 0x0FFF); // CRC byte is replaced by 0
    n_prom[7] = 0; // Subsidiary value, set to 0
    for (cnt = 0; cnt < 16; cnt++) // operation is performed on bytes
    { // choose LSB or MSB
        if (cnt % 2 == 1)
            n_rem ^= (uint16_t) ((n_prom[cnt >> 1]) & 0x00FF);
        else
            n_rem ^= (uint16_t) (n_prom[cnt >> 1] >> 8);
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
