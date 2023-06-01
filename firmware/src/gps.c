/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: gps.c                                     //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: GPS functions                      //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "gps.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "crsf.h"
#include "usart.h"
#include "settings.h"


TaskHandle_t gpsTaskHandle;
QueueHandle_t sentenceQueueHandle;
QueueHandle_t gpsPacketQueueHandle;
bool gpsHealthy;

#define NUM_BUFFERS 5

static char buffer[NUM_BUFFERS][80];
static volatile int currentBuffer = 0;
static volatile int pos = 0;
static int tokens[20];

static CRSF_sensor_gps gpsData;

bool parseNMEASentence(char *nmea);
void tokenize(char *str);

void gpsISR(void) {
    uint8_t rx;
    rx = SERCOM1_REGS->USART_INT.SERCOM_DATA;
    if (pos == 0) {
        if (rx == '$') {
            buffer[currentBuffer][pos] = rx;
            ++pos;
        }
    } else {
        buffer[currentBuffer][pos] = rx;
        if (rx == '\n') {
            buffer[currentBuffer][pos] = '\0';
            BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
            char *p = buffer[currentBuffer];
            xQueueSendToBackFromISR(sentenceQueueHandle, (void *) &p, &pxHigherPriorityTaskWoken);
            portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
            pos = 0;
            ++currentBuffer;
            if (currentBuffer >= NUM_BUFFERS) {
                currentBuffer = 0;
            }
        } else {
            if (pos < 80) {
                ++pos;
            }
        }
    }
}

void gpsTask(void *pvParameters) {
    char *nmeaSentence;
    switchSERCOM1Baud(settings.gpsBaud);
    gpsHealthy = false;
    NVIC_SetPriority(SERCOM1_IRQn, 1);
    SERCOM1_REGS->USART_INT.SERCOM_INTENSET = (uint8_t) (SERCOM_USART_INT_INTENSET_ERROR_Msk | SERCOM_USART_INT_INTENSET_RXC_Msk);
    NVIC_EnableIRQ(SERCOM1_IRQn);
    while (1) {
        if (xQueueReceive(sentenceQueueHandle, &nmeaSentence, portMAX_DELAY) == pdTRUE) {
            if (parseNMEASentence(nmeaSentence)) {
                xQueueOverwrite(gpsPacketQueueHandle, &gpsData);
            }
        }
    }
}

void tokenize(char *str) {
    int token = 0;
    int pos = 0;
    while (pos < 80 && str[pos]) {
        if (str[pos] == ',') {
            str[pos] = '\0';
            tokens[token++] = pos + 1;
        }
        ++pos;
    }
}

int32_t convertLatLong(float f) {
    int32_t temp32;
    float degrees = ((int) f) / 100;
    f -= degrees * 100;
    f /= 60.0;
    f = degrees + f;
    temp32 = (f * 10000000);
    return ((temp32 >> 24) & 0x000000ff) | ((temp32 >> 8) & 0x0000ff00) 
            | ((temp32 << 8) & 0x00ff0000) | (temp32 << 24);
}

bool parseNMEASentence(char *nmea) {
    float f;
    uint16_t temp16;
    bool update = false;
    if (strncmp(nmea, "$GPGGA", 6) == 0) {
        tokenize(nmea);
        if (nmea[tokens[5]] == '0') {
            gpsHealthy = false;
        } else {
            gpsHealthy = true;
            f = atof(&nmea[tokens[1]]);
            if (nmea[tokens[2]] == 'S') {
                f = -f;
            }
            gpsData.latitude = convertLatLong(f);
            f = atof(&nmea[tokens[3]]);
            if (nmea[tokens[4]] == 'W') {
                f = -f;
            }
            gpsData.longitude = convertLatLong(f);
            gpsData.satellites = atoi(&nmea[tokens[6]]);
            f = atof(&nmea[tokens[8]]);
            gpsData.altitude = (uint16_t) (round(f) + 1000);
            gpsData.altitude = (gpsData.altitude >> 8) | (gpsData.altitude << 8);
            update = true;
        }
    } else if (gpsHealthy && strncmp(nmea, "$GPVTG", 6) == 0) {
        tokenize(nmea);
        f = atof(&nmea[tokens[0]]);
        temp16 = f * 100;
        gpsData.heading = (temp16 >> 8) | (temp16 << 8);
        f = atof(&nmea[tokens[6]]);
        temp16 = round(f * 10); 
        gpsData.groundspeed = (temp16 >> 8) | (temp16 << 8);
    }
    return update;
}
