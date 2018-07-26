#ifndef __STM32F10_USART_H
#define __STM32F10_USART_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h" 
#include <stdio.h>
#include <string.h> 


void Usart1_Rx(void);
void Usart1_Tx(void);
void Usart2_Rx(void);
void Usart2_Tx(void); 
void Usart1ST(void);
void Usart2ST(void);
void UsartSP(void);
void UartSP(void);

extern void delay_ms(u16 nms);
extern void delay_us(u32 nus); 

void Usart_Configuration(void);
void NVIC_Configuration(void);
void UsartRcc_Configuration(void);
void UsartGPIO_Configuration(void);  

#endif /*_USART_H*/
