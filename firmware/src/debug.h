/* 
 * File:   debug.h
 * Author: bemcg
 *
 * Created on April 4, 2023, 8:38 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DEBUG_MAX_MSG_LEN   30   
    
    void debugMsg(const char *msg, ...);


#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

