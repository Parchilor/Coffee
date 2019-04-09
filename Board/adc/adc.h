#ifndef __ADC_H_
#define __ADC_H_

#include <stdint.h>

#define ADC_COIL    0
#define ADC_MOTOR   1

void System_ADCInit(void);
int32_t ADC_ReadValue(uint8_t idx);
#endif