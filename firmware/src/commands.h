/* 
 * File:   commands.h
 * Author: bemcg
 *
 * Created on April 9, 2023, 2:41 PM
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define DEVICE_SIGNATURE        0x454c5253  //ELRS
    enum {
        CMD_GET_VERSION = 0x80, CMD_GET_SETTINGS = 0x81, CMD_GET_VOLTAGE = 0x82,
        CMD_SAVE_SETTINGS = 0xa0, CMD_SET_VOLTAGE = 0xa1
    };


#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDS_H */

