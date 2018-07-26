#ifndef __CONFIGH_H
#define __CONFIGH_H

#include "stm32f10x.h"
//data---���ź�ֵ��value---��������ֵ
extern float  TSRegister[12];
extern float  Registers[16];                 //�Ĵ���ֵ,�洢�Ƕ˿�������
extern float  ADC_value[12];                 //ADC��������ֵ
extern float  ADC_data[12];                  //ADC���ź�ֵ
extern float  RTD_value[4];
extern float  DAC_value[8];
extern float  DAC_data[8];
extern u8   	DI_data[8];
extern u8     DO_data[8];

extern char localControl;
extern char localDeviceRun;


extern char ReadPortsBuffer[165];
extern char WritePortsBuffer[104];
extern char ReadPIDBuffer[96];
extern char WritePIDBuffer[97]; //HandleBit(1),PID1---PID4(24/each);

extern float  Setpoint[3];

unsigned int  ReceivedNum = 0;
char  ReceivedBuffer[4000];
char  SentBuffer[200];

#endif
