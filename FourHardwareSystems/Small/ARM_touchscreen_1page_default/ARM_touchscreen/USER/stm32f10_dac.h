#ifndef __STM32F10_DAC_H
#define __STM32F10_DAC_H

#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


#include <stdint.h>

#define NUM_DAC	8

struct _dac_reg{
	uint8_t addr;
	uint16_t reg;
};

void dac_init(void);
uint8_t dac_read_write_byte(uint8_t c);
void dac_write(struct _dac_reg *conf);
void APortOut(u8 port,float dacvl,float range);
void APortCOut(float dacvl[8],float range[8]);


#endif /*_DAC_H*/

