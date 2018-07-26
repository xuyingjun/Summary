#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f10x.h"
#include "delay.h"
#include "AD.h"
#include "Dio.h"
#include "W25Q16.h"
#include "DAC8554.h"
#include "Globle.h"
#include "usart.h"
#include "timer.h"

//_TempData tempdata;

void RCC_Configuration(void);
void NVIC_Configuration(void);
void LedInit(void);

#endif
