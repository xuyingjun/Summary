#ifndef DAC_H_
#define DAC_H_

#include <stdint.h>
#include "stm32f10x.h"
#include <stdio.h>

void DAC_init(void);
void APortCOut(float* value);
void APortOut(uint8_t channel, float value);

#endif 
