/* 
 * File:   ms5637.h
 * Author: bemcg
 *
 * Created on April 3, 2023, 3:04 PM
 */

#ifndef MS5637_H
#define	MS5637_H

#ifdef	__cplusplus
extern "C" {
#endif

    void ms5637Init(void);
    float getPressure(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MS5637_H */

