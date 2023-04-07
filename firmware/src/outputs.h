/* 
 * File:   outputs.h
 * Author: bemcg
 *
 * Created on April 6, 2023, 9:40 PM
 */

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

