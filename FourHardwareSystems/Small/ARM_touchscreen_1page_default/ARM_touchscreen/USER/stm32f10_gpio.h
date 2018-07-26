#ifndef __STM32F10_GPIO_H
#define __STM32F10_GPIO_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void PORTRcc_Configuration(void);
void PORT_Configuration(void);
void port_Configuration(void);


u8 DPortIn(u8 port);
void DPortCOut(u8 arr[8]);
void DPortCIn(u8 arr[8]); 
void DPortOut(u8 port,u8 value);
#endif
