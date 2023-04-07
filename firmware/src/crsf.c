#include "definitions.h"
#include "crsf.h"
#include "crsf_uart.h"

static uint8_t crsfTxBuffer[MAX_CRSF_PACKET];
static uint8_t _lut[256];

void initCRC(uint8_t poly);
void T3Callback(TC_TIMER_STATUS status, uintptr_t context);

void initCRSF(void) {
    initCRC(0xd5);
    TC3_TimerCallbackRegister(T3Callback, (uintptr_t) NULL);
    lastByteTick = 0;
    synched = false;
    TC3_TimerStart();
    SERCOM0_REGS->USART_INT.SERCOM_INTENSET = (uint8_t) (SERCOM_USART_INT_INTENSET_ERROR_Msk | SERCOM_USART_INT_INTENSET_RXC_Msk);
}

void T3Callback(TC_TIMER_STATUS status, uintptr_t context) {
    ++lastByteTick;
}

void initCRC(uint8_t poly) {
    for (int idx = 0; idx < 256; ++idx) {
        uint8_t crc = idx;
        for (int shift = 0; shift < 8; ++shift) {
            crc = (crc << 1) ^ ((crc & 0x80) ? poly : 0);
        }
        _lut[idx] = crc & 0xff;
    }
}

uint8_t calcCRC(uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    while (len--) {
        crc = _lut[crc ^ *data++];
    }
    return crc;
}

enum {BATTERY_PACKET_LEN = 12, BATTERY_CRC_OFFSET = 11, BATTERY_PAYLOAD_LEN = 10};

void sendBatteryTelem(float voltage) {
    if (!txIsBusy()) {
        CRSF_Frame *frame = (CRSF_Frame *) crsfTxBuffer;
        frame->address = CRSF_ADDRESS_FLIGHT_CONTROLLER;
        frame->length = BATTERY_PAYLOAD_LEN;
        frame->type = CRSF_FRAMETYPE_BATTERY_SENSOR;
        CRSF_sensor_battery *sensor = (CRSF_sensor_battery *) frame->data;
        sensor->capacity = 0;
        sensor->current = 0;
        sensor->remaining = 100;
        uint16_t v = voltage * 10;
        sensor->voltage = (v  >> 8) | (v << 8);
        crsfTxBuffer[BATTERY_CRC_OFFSET] = calcCRC(&crsfTxBuffer[2], BATTERY_PAYLOAD_LEN - 1);
        writeUart(crsfTxBuffer, BATTERY_PACKET_LEN);
    }
}

enum {ALT_PACKET_LEN = 8, ALT_CRC_OFFSET = 7, ALT_PAYLOAD_LEN = 6};

//altitude in meters, vspeed in m/s
void sendAltitudeTelem(float altitude, float vSpeed) {
    if (!txIsBusy()) {
        CRSF_Frame *frame = (CRSF_Frame *) crsfTxBuffer;
        frame->address = CRSF_ADDRESS_FLIGHT_CONTROLLER;
        frame->length = ALT_PAYLOAD_LEN;
        frame->type = CRSF_FRAMETYPE_BARO_ALTITUDE;
        CRSF_sensor_baro_vario *sensor = (CRSF_sensor_baro_vario *) frame->data;
        uint16_t temp = altitude * 10 + 10000;
        sensor->altitude = (temp >> 8) | (temp << 8);
        temp = vSpeed * 100;
        sensor->verticalspd = (temp >> 8) | (temp << 8);
        crsfTxBuffer[ALT_CRC_OFFSET] = calcCRC(&crsfTxBuffer[2], ALT_PAYLOAD_LEN - 1);
        writeUart(crsfTxBuffer, ALT_PACKET_LEN);
    }
}