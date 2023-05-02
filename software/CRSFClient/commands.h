#ifndef COMMANDS_H
#define	COMMANDS_H

#ifdef	__cplusplus
extern "C" {
#endif
    #define DEVICE_SIGNATURE        0x454c5253  //ELRS

    enum {
        CMD_GET_VERSION = 0x80, CMD_GET_SETTINGS = 0x81,
        CMD_SAVE_SETTINGS = 0xa0
    };


#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDS_H */

