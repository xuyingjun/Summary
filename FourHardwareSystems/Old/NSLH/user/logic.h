#ifndef __LOGIC_H
#define __LOGIC_H

#include "stm32f10x.h"
#include "digital_io.h"
#include "systick_timer.h"
#include "adc.h" 
#include "dac.h"
#include "rtd.h"
#include "timer.h"
#include "algorithm.h"

#include "ChangingPara.h"

extern float dataTemp[12];

////AI
//#define DIFFPRE                   	3										   //ѹ��
//#define CHWVfb                      11			               //��ˮ������ֵ   

//RTD
#define CHWOutTemp                  2                    //��ˮ�����¶�

//DI
//1---Occupy State     0---Unoccupied mode 
#define LOCALORREMOTE               0                    //remote����localģʽ
#define OccuState                   1                    //����Occupined Sensor����ȡ����״̬�����˻�����
#define BMSSignal                   2                    //BMS �����ź�  1---Start  0---OFF
#define FANFault                    3										 //Fan fault

////AO
//#define PTBFan1                     0                    //����PTB����
//#define ERVFan	                    7                    //����ERV����
//#define CHWV                        5                    //������ˮ������
//#define PTBFan2                    	3                    //����PTB���ȣ��󷿼���6̨PTB���������˿ڿ���

////DO
//#define NSLHAlarm                   2                    //������Ϣ

extern u8 TimeFlag;
extern u8 UnOccupined;
extern u8 ReceiveFlag;
extern u8 TSControl;
extern u8 TempPassive;

typedef enum _startselection
{
  TSstart   = 0x01,             //��ʾ������,��������
  BMSStart  = 0x02,	           //BMS���� 
  OccuStart = 0x03,	           //Occupined Sensor ����ģʽ  
}STARTSEL;

//ϵͳ״̬��־λ
extern u8 systemStatus;

extern float ADC_data[12];
extern float ADC_value[12];
extern float RTD_value[4];
extern float DAC_data[8];
extern u8 DI_data[8];
extern u8 DO_data[8];

extern u8 TimeFlag;
extern u8 UnOccupined;
extern u8 ReceiveFlag;
extern u8 TSControl;
extern u8 TempPassive;
extern u8 BMSControl;

extern float off_coilTemp;
extern float off_coilTempSv;
extern float chwInTemp;
extern float chwOutTemp;
extern u16 RoomTemp;
extern u16 RoomTempSv;
extern u16 CO2;
extern u16 CO2Sv;
extern float valve;
extern float FanSpeed;
extern float VavleFb;
extern float DiffPressure;

extern u8 PTBFANRUN;
extern u8 ERVFANRUN;

extern u16 ERVState;

u8 NSLH_Alarm(void);
u8 NSLH_Start(void);
u8 NSLH_Stop(void);
void NSLH_Clear(void);

void Get_value(float chwOut,float diffPre,u8 BMS);

void CO2Control(void);
void RoomTempControl(float value);
u8 OccupanyMode(void);   //����ģʽ
u8 Unoccupanied(void);   //����ģʽ

#endif
