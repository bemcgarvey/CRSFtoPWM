/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: sensorTask.c                              //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: sensor task handler                //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "sensorTask.h"
#include "sensors.h"
#include "settings.h"
#include "debug.h"
#include "crsf.h"
#include "rtosHandles.h"

TaskHandle_t sensorTaskHandle;
#define INITIAL_ALT_SAMPLES     4

void sensorTask(void *pvParameter) {
    vTaskDelay(100);
    initSensors();
    int delay;
    float dAlt = 0;
    float groundLevel = 0;
    float lastAltitude = 0;
    int initSamples = INITIAL_ALT_SAMPLES;
    delay = 500 / settings.sensorRate;
    debugMsg("Sensors initialized");
    if (altimeterHealthy) {
        debugMsg("Altimeter good");
    } else {
        debugMsg("Altimeter failed");
    }
    while (1) {
        vTaskDelay(delay);
        if (altimeterHealthy) {
            float alt = getAltitude();
            if (initSamples > 0) {
                --initSamples;
                groundLevel += alt;
                if (initSamples == 0) {
                    groundLevel /= INITIAL_ALT_SAMPLES;
                }
            }
            if (initSamples == 0) {
                alt = alt - groundLevel;
                dAlt = alt - lastAltitude;
                lastAltitude = alt;
                dAlt *= settings.sensorRate;
            }
            sendAltitudeTelem(alt, dAlt);
        }
        vTaskDelay(delay);
        float v = getVBat();
        sendBatteryTelem(v);
        xTaskNotify(watchdogTaskHandle, SENSOR_TASK_NOTIFY, eSetBits);
    }
}
