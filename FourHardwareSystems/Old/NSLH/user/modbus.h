#ifndef __MODBUS_H
#define __MODBUS_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "Globe.h"


//����ȫ�ֱ���
extern u8 RS485_RX_BUFF[128];               //���ջ�����2048�ֽ�
extern u16 RS485_RX_CNT;                   //���ռ�����
extern u8 RS485_FrameFlag;                 //֡�������
extern u8 RS485_TX_BUFF[128];               //���ͻ�����2048�ֽ�
extern u16 RS485_TX_CNT;                   //���ͼ�����

//Modbus�Ĵ����͵�Ƭ���Ĵ���ӳ���ϵ
extern vu32 *Modbus_InputIO[100];            //���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
extern vu32 *Modbus_OutputIO[100];           //����������Ĵ���ָ��(����ʹ�õ���λ������)
extern u16  *Modbus_HoldReg[128];            //���ּĴ���

extern u16 CO2Value;
extern u16 CO2ValueSv;
extern u16 RoomTempPv;
extern u16 RoomTempSetValue;
extern u16 RoomHumPv;
extern u16 RoomHumSetValue;
extern u16 CHWTemp;
extern u16 DPValue;
extern u16 ValveFb;
extern u16 OccSta;
extern u16 ECS;

extern u16 IBSCommand;
extern u16 ScheduleComm;
extern u16 PDVComm;

extern u16 ValveCmd;							// 14, ������ȫ�������ִ�п����߼�
extern u16 ERVCmd;							  // 15; ERV����ȫ�������ִ�п����߼�
extern u16 ERVState;							// 16; ERV״̬������0Ϊ�أ�1Ϊȫ����������ֵΪ����2-100

extern u16 PurgeCmd;							// 17; Purge�������0Ϊ�أ�1Ϊȫ����������10����
extern u16 TempSPSet_Unocc;				// 18; ����ģʽ���¶��趨ֵ	 
extern u16 SpareSet1;							// 19; ���ò�������1��0-100
extern u16 SpareSet2;							// 20; ���ò�������2��0-100
extern u16 SpareSet3;							// 21; ���ò�������3��0-100

extern u16 CO2;
extern u16 CO2Sv;
extern u16 RoomTemp;
extern u16	RoomTempSv;
extern u16 RoomHum;

//������غ���
void Modbus_RegMap(void);
void RS485_Service(void);
void Modbus_02_Solve(void);
void Modbus_01_Solve(void);
void Modbus_05_Solve(void);
void Modbus_15_Solve(void);
void Modbus_03_Solve(void);
void Modbus_06_Solve(void);
void Modbus_16_Solve(void);

#endif
