/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: outputs.h                                 //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: PWM output control                 //
/////////////////////////////////////////////////////

#ifndef OUTPUTS_H
#define	OUTPUTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    void enableOutputs(void);
    void disableThrottle(void);
    void setPWMfrequency(int freq);
    void setOutput(int out, uint32_t value);

#ifdef	__cplusplus
}
#endif

#endif	/* OUTPUTS_H */

