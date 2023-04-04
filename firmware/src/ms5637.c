#include "definitions.h"
#include "ms5637.h"

#define MS5637_ADDRESS  0b1110110
enum {CMD_RESET = 0x1e, CMD_PROM_READ = 0xa0};

static uint8_t cmd;
static uint16_t rxBuffer[8];

void ms5637Reset(void) {
    cmd = CMD_RESET;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
}
void ms5637ReadCalibration(void) {
    cmd = CMD_PROM_READ;
    SERCOM3_I2C_Write(MS5637_ADDRESS, &cmd, 1);
    while (SERCOM3_I2C_IsBusy());
    SERCOM3_I2C_Read(MS5637_ADDRESS, (uint8_t *)rxBuffer, 2);
    while (SERCOM3_I2C_IsBusy());
    printf("%04x\r\n", rxBuffer[0]);
}

void ms5637Init(void) {
    ms5637Reset();
    ms5637ReadCalibration();
}

unsigned char crc4(unsigned int n_prom[]) // n_prom defined as 8x unsigned int (n_prom[8])
{
    int cnt; // simple counter
    unsigned int n_rem = 0; // crc reminder
    unsigned char n_bit;
    n_prom[0] = ((n_prom[0]) & 0x0FFF); // CRC byte is replaced by 0
    n_prom[7] = 0; // Subsidiary value, set to 0
    for (cnt = 0; cnt < 16; cnt++) // operation is performed on bytes
    { // choose LSB or MSB
        if (cnt % 2 == 1) n_rem ^= (unsigned short) ((n_prom[cnt >> 1]) & 0x00FF);
        else n_rem ^= (unsigned short) (n_prom[cnt >> 1] >> 8);
        for (n_bit = 8; n_bit > 0; n_bit--) {
            if (n_rem & (0x8000)) n_rem = (n_rem << 1) ^ 0x3000;
            else n_rem = (n_rem << 1);
        }
    }
    n_rem = ((n_rem >> 12) & 0x000F); // final 4-bit reminder is CRC code
    return (n_rem ^ 0x00);
}
