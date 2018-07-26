#ifndef _DIO_H_
#define _DIO_H_
#include "stm32f10x.h"
#define Di_Port   GPIOF
#define Di0_Pin    GPIO_Pin_8
#define Di1_Pin    GPIO_Pin_9
#define Di2_Pin    GPIO_Pin_10
#define Di3_Pin    GPIO_Pin_11
#define Di4_Pin    GPIO_Pin_12
#define Di5_Pin    GPIO_Pin_13
#define Di6_Pin    GPIO_Pin_14
#define Di7_Pin    GPIO_Pin_15

#define Do_Port    GPIOE
#define Do1_Pin    GPIO_Pin_8
#define Do2_Pin    GPIO_Pin_9
#define Do3_Pin    GPIO_Pin_10
#define Do4_Pin    GPIO_Pin_11
#define Do5_Pin    GPIO_Pin_12
#define Do6_Pin    GPIO_Pin_13
#define Do7_Pin    GPIO_Pin_14
#define Do8_Pin    GPIO_Pin_15
#define SpdtON 1
#define SpdtOFF 0


u8 GetDiMap(void);
void SetSpdtState(u8 channel,u8 State);
void DioCfg(void);
#endif

