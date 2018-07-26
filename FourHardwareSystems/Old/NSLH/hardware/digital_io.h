#ifndef __DIGITAL_IO_H
#define __DIGITAL_IO_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GpioRcc_Configuration(void);
void Gpio_Configuration(void);
void IO_Configuration(void);
void DPortOut(u8 port,u8 value);
u8 DPortIn(u8 port);
void DPortCOut(u8 arr[8]);
void DPortCIn(u8 arr[8]);
#endif
