#ifndef _PWM_H_
#define _PWM_H_
#include "stm32f10x.h"
void PWM1_Init(u16 arr);  //arr Ϊ���� 1LSB=100us  
void PWM1_SetHighTime(u16 HighTime);     //PWM2 ��������ߵ�ƽʱ��
void PWM2_Init(u16 arr);  //arr Ϊ���� 1LSB=100us 
void PWM2_SetHighTime(u16 HighTime);     //PWM2 ��������ߵ�ƽʱ��
#endif

