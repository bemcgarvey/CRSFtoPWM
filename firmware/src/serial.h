/* 
 * File:   serial.h
 * Author: bemcg
 *
 * Created on April 8, 2023, 3:54 PM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

    bool detectSerial(void);
    void serialMain(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

