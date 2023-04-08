#include "definitions.h"
#include "outputs.h"
#include "settings.h"
#include "rxTask.h"
#include "debug.h"

/*
 *       RC     PWM
 * min  172 ->  988us
 * mid  992 -> 1500us
 * max 1811 -> 2012us
 */
#define CRSF_MAX    1811
#define CRSF_MIN    172
#define CRSF_MID    992
#define SERVO_COUNT_PER_MS          3000
#define SERVO_COUNT_PER_MS_CH9_CH10 3200
#define SERVO_LOW   988
#define SERVO_HIGH  2012

void enableOutputs(void) {
    TCC0_CompareStart();
    TCC1_CompareStart();
    TCC2_CompareStart();
    TC5_CompareStart();
}

void disableThrottle(void) {
    switch(settings.throttleChannel) {
        case 0: TCC1_Compare24bitMatchSet(0, 0);
        break;
        case 1: TCC1_Compare24bitMatchSet(1, 0);
        break;
        case 2: TCC0_Compare24bitMatchSet(0, 0);
        break;
        case 3: TCC0_Compare24bitMatchSet(1, 0);
        break;
    }
}

void setPWMfrequency(int freq) {
    uint16_t period;
    if (freq < 50) {
        freq = 50;  //minimum frequency is 50Hz
    }
    period = 3000000 / freq;
    TCC0_Compare24bitPeriodSet(period);
    TCC1_Compare24bitPeriodSet(period);
    TCC2_Compare16bitPeriodSet(period);
}

void setOutput(int out, uint32_t value) {
    //TODO can these calculations be improved
    if (out < 8) {
        value = (value - CRSF_MIN) * (SERVO_HIGH - SERVO_LOW);
        value += SERVO_LOW * (CRSF_MAX - CRSF_MIN);
        value *= SERVO_COUNT_PER_MS / 100;
        value /= 10 * (CRSF_MAX - CRSF_MIN);
    } else {
        value = (value - CRSF_MIN) * (SERVO_HIGH - SERVO_LOW);
        value += SERVO_LOW * (CRSF_MAX - CRSF_MIN);
        value *= SERVO_COUNT_PER_MS_CH9_CH10 / 100;
        value /= 10 * (CRSF_MAX - CRSF_MIN);
    }
    if (out < 2) {
        TCC1_Compare24bitMatchSet(out, value);
    } else if (out < 6) {
        TCC0_Compare24bitMatchSet(out - 2, value);
    } else if (out < 8) {
        TCC2_Compare16bitMatchSet(out - 6, value);
    } else if (out == 8) {
        TC5_Compare16bitMatch0Set(value);
    } else if (out == 9) {
        TC5_Compare16bitMatch1Set(value);
    }
}