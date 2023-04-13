/* 
 * File:   rtosHandles.h
 * Author: bemcg
 *
 * Created on April 3, 2023, 8:05 PM
 */

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

extern QueueHandle_t debugQueue;
extern QueueHandle_t packetQueue;

#ifdef	__cplusplus
}
#endif

#endif	/* RTOSHANDLES_H */

