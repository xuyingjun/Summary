#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"
#include <stdio.h>

void ADC_init(void);
void APortIn(char channel,float *adc_value);
void APortCIn(float *adc_value);

#endif
