/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: gps.h                                     //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: GPS functions                      //
/////////////////////////////////////////////////////

#ifndef GPS_H
#define	GPS_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern bool gpsHealthy;
    void gpsTask(void *pvParameters);
    void gpsISR(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GPS_H */

