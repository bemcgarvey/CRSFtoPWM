//TODO header

#ifndef PASSTHROUGHTASK_H
#define	PASSTHROUGHTASK_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern volatile bool passthroughEnabled;
    void passthroughTask(void *pvParameters);
    void passthroughISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PASSTHROUGHTASK_H */

