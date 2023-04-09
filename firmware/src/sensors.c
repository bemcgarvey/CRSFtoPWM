#include "definitions.h"
#include "sensors.h"
#include "ms5637.h"
#include "debug.h"
#include "settings.h"
#include <math.h>

void initSensors(void) {
    ms5637Init();
}

float getAltitude(void) {
    int p = getPressure();
    float alt;
    alt = p / 101325.0;
    alt = pow(alt, (1.0 / 5.255));
    alt = 44330 * (1 - alt);
    return alt;
}

#define ADC_COUNT_TO_VOLT   ((3.3 / 4095) * 7.8)

float getVBat(void) {
    ADC_Enable();
    ADC_ConversionStart();
    while (!ADC_ConversionStatusGet());
    int result = ADC_ConversionResultGet();
    ADC_Disable();
    return (result * ADC_COUNT_TO_VOLT) + settings.batCalibration;
}
