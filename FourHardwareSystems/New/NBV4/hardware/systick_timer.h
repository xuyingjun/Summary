#ifndef	_SYSTICK_INIT_
#define _SYSTICK_INIT_

#include "core_cm3.h"

//void SysTick_init(void);
void delay_ms(__IO uint32_t nTime);
void delay_us(__IO uint32_t nTime);
void delay_s(__IO uint32_t nTime);
#endif /*_SYSTICK_INIT_*/



