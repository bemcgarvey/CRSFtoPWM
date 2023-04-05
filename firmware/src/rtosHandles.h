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

extern TaskHandle_t statusTaskHandle;
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t packetTaskHandle;

#define DEBUG_MAX_MSG_LEN   30
extern QueueHandle_t debugQueue;

#ifdef	__cplusplus
}
#endif

#endif	/* RTOSHANDLES_H */
