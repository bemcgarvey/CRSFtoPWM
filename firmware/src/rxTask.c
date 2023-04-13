/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: rxTask.c                                  //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: rx task handler                    //
/////////////////////////////////////////////////////

#include "definitions.h"
#include "rxTask.h"
#include "rtosHandles.h"
#include "crsf.h"
#include "debug.h"
#include "outputs.h"
#include "settings.h"

TaskHandle_t rxTaskHandle;
static uint8_t packetBuffer[CHANNEL_PACKET_LEN];
uint16_t channels[NUM_CHANNELS];

void handleChannelPacket(void);
void handleLinkPacket(void);

unsigned int packetCount = 0;

void rxTask(void *pvParameters) {
    debugMsg("Starting rx task");
    initCRSF();
    setPWMfrequency(settings.servoRate);
    enableOutputs();
    while (1) {
        if (xQueueReceive(packetQueue, packetBuffer, 10) == pdTRUE) {
            ++packetCount;
            if (packetCount % 100 == 0) {
                debugMsg("*");
            }
            CRSF_Frame *frame = (CRSF_Frame *) packetBuffer;
            if (frame->address == CRSF_ADDRESS_FLIGHT_CONTROLLER) {
                if (frame->type == CRSF_FRAMETYPE_RC_CHANNELS_PACKED) {
                    handleChannelPacket();
                } else if (frame->type == CRSF_FRAMETYPE_LINK_STATISTICS) {
                    handleLinkPacket();
                }
            }
        }
        xTaskNotify(watchdogTaskHandle, RX_TASK_NOTIFY, eSetBits);
    }
}

void handleChannelPacket(void) {
    CRSF_Frame *frame = (CRSF_Frame *) packetBuffer;
    CRSF_ChannelData *ch = (CRSF_ChannelData*)&(frame->data);
    channels[0] = ch->ch0;
    channels[1] = ch->ch1;
    channels[2] = ch->ch2;
    channels[3] = ch->ch3;
    channels[4] = ch->ch4;
    channels[5] = ch->ch5;
    channels[6] = ch->ch6;
    channels[7] = ch->ch7;
    channels[8] = ch->ch8;
    channels[9] = ch->ch9;
    channels[10] = ch->ch10;
    channels[11] = ch->ch11;
    channels[12] = ch->ch12;
    channels[13] = ch->ch13;
    channels[14] = ch->ch14;
    channels[15] = ch->ch15;
    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        setOutput(i, channels[settings.ouputMap[i]]);
    }
    failsafeEnabled = false;
}

void handleLinkPacket(void) {
    CRSF_Frame *frame = (CRSF_Frame *) packetBuffer;
    CRSF_LinkStatistics *linkStats = (CRSF_LinkStatistics *)&(frame->data);
    if (linkStats->uplink_Link_quality == 0) {
        if (!failsafeEnabled) {
            failsafeEnabled = true;
            if (settings.failsafeMode == FAILSAFE_THROTTLE_OFF) {
                disableThrottle();
            }
            debugMsg("Failsafe engaged RSSI = %d", linkStats->uplink_RSSI_1);
        }
    }
}