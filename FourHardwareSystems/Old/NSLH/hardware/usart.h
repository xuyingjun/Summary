#ifndef _USART_H
#define _USART_H
#include <stdio.h>
#include "stm32f10x.h"
#include "misc.h"

void  Usart_Total_Initi(void);
void  Usart_SendChar(char dat);
void 	Usart2_SendChar(char dat);
char  Usart_GetChar(USART_TypeDef* USARTx);
void  UsartSendData(int num,char* sendbuffer);
void 	Usart2SendData(int num,char* sendbuffer);
void  NVIC_Configuration(void);
void  Usart_GPIO_Configuration(void);
void  delay_usart(void);
void  TurnOnUsart1(void);
void  TurnOnUsart2(void);
void  TurnOffUsart1(void);
void  TurnOffUsart2(void);
void UsendChar(char dat);
void UsartSendString(u8 arr[]);

#endif
