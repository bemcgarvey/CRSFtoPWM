/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: sensors.h                                 //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: sensor functions                   //
/////////////////////////////////////////////////////

#ifndef SENSORS_H
#define	SENSORS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define PRESSURE_SAMPLE_MS      40
    void initSensors(void);
    float getAltitude(void);
    float getVBat(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSORS_H */

