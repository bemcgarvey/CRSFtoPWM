/* 
 * File:   rxTask.h
 * Author: bemcg
 *
 * Created on April 5, 2023, 8:48 PM
 */

#ifndef RXTASK_H
#define	RXTASK_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_CHANNELS    16

    extern uint16_t channels[NUM_CHANNELS];
    extern unsigned int packetCount;
    
    void rxTask(void *pvParameters);

#ifdef	__cplusplus
}
#endif

#endif	/* RXTASK_H */

