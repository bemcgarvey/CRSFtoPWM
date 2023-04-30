/* 
 * File:   passthroughTask.h
 * Author: bemcg
 *
 * Created on April 28, 2023, 2:00 PM
 */

#ifndef PASSTHROUGHTASK_H
#define	PASSTHROUGHTASK_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern volatile bool passthroughEnabled;
    void passthroughTask(void *pvParameters);
    void passthroughRxTask(void *pvParameters);


#ifdef	__cplusplus
}
#endif

#endif	/* PASSTHROUGHTASK_H */

