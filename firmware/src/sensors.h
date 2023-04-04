/* 
 * File:   sensors.h
 * Author: bemcg
 *
 * Created on April 3, 2023, 10:05 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#ifdef	__cplusplus
extern "C" {
#endif

    void initSensors(void);
    float getAltitude(void);
    float getVBat(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSORS_H */

