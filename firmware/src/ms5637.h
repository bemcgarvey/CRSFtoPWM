/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: ms5637.h                                  //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: ms5637 sensor functions            //
/////////////////////////////////////////////////////

#ifndef MS5637_H
#define	MS5637_H

#ifdef	__cplusplus
extern "C" {
#endif

    void ms5637Init(void);
    int32_t getPressure(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MS5637_H */

