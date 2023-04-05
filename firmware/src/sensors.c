#include "definitions.h"
#include "sensors.h"
#include "ms5637.h"

void initSensors(void) {
    ms5637Init();
}

float getAltitude(void) {
    return 0;
}

#define ADC_COUNT_TO_VOLT   ((3.3 / 4095) * 7.8)

float getVBat(void) {
    ADC_Enable();
    ADC_ConversionStart();
    while (!ADC_ConversionStatusGet());
    int result = ADC_ConversionResultGet();
    ADC_Disable();
    return result * ADC_COUNT_TO_VOLT;
}
