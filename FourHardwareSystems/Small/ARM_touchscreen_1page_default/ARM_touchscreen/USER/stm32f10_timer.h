#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


void TimeRcc_Configuration(void);

void Time_Configuration(void);
void Tim3ST(void);
void Tim3SP(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void delay_s(u8 ns);
void Tim4ST(void);
void Tim4SP(void);
void Tim1ST(void);
void Tim1SP(void);
void Tim2ST(void);
void Tim2SP(void);
void Tim5ST(void);
void Tim5SP(void);

#endif
