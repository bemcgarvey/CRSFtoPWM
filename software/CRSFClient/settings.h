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

#include <stdint.h>

#define NUM_OUTPUTS 10
    
    enum {FAILSAFE_THROTTLE_OFF = 0, FAILSAFE_HOLD_ALL = 1};
    enum {UART_DEBUG = 0, UART_GPS = 1, UART_BOOTLOADER = 2};
    enum {I2C_NONE = 0, I2C_COMPASS = 1, I2C_AIRSPEED = 2, I2C_OTHER = 4};
    
    typedef struct {
        uint8_t ouputMap[NUM_OUTPUTS];
        uint16_t servoRate;
        uint8_t throttleChannel;
        bool sBusEnabled;
        uint8_t sensorRate;
        uint8_t failsafeMode;
        uint8_t uartMode;
        uint8_t i2cSensors;
        float batCalibration;
    } Settings;
    
#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_H */

