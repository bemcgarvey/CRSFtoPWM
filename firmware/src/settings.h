/* 
 * File:   settings.h
 * Author: bemcg
 *
 * Created on April 4, 2023, 2:17 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_OUTPUTS 10
    typedef struct __attribute__((packed)) {
        uint8_t ouputMap[NUM_OUTPUTS];
        uint16_t servoRate;
        uint8_t throttleChannel;
        bool sBusEnabled;
        uint8_t sensorRate;
        uint8_t failsafeMode;
        uint8_t uartMode;
        //TODO add battery calibration value?
    } Settings;

    extern Settings settings;
    extern bool altimeterHealthy;
    extern bool failsafeEnabled;
    
    bool loadSettings(void);
    bool saveSettings(void); 
#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_H */

