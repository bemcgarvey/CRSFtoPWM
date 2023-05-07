/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: rtosHandles.h                             //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: freeRTOS task and queue handles    //
/////////////////////////////////////////////////////

#ifndef RTOSHANDLES_H
#define	RTOSHANDLES_H

#ifdef	__cplusplus
extern "C" {
#endif

    enum {RX_TASK_NOTIFY = 0x01, SBUS_TASK_NOTIFY = 0x02, SENSOR_TASK_NOTIFY = 0x04};
extern TaskHandle_t statusTaskHandle;
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t rxTaskHandle;
extern TaskHandle_t sbusTaskHandle;
extern TaskHandle_t watchdogTaskHandle;
extern TaskHandle_t passthroughTaskHandle;
extern TaskHandle_t gpsTaskHandle;

extern QueueHandle_t debugQueue;
extern QueueHandle_t packetQueue;
extern QueueHandle_t passthroughQueueS0toS1;
extern QueueHandle_t passthroughQueueS1toS0;
extern QueueHandle_t sentenceQueueHandle;
extern QueueHandle_t gpsPacketQueueHandle;

#ifdef	__cplusplus
}
#endif

#endif	/* RTOSHANDLES_H */

