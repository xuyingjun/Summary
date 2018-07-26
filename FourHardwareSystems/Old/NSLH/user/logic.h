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
//#define DIFFPRE                   	3										   //压差
//#define CHWVfb                      11			               //冷水阀反馈值   

//RTD
#define CHWOutTemp                  2                    //冷水出口温度

//DI
//1---Occupy State     0---Unoccupied mode 
#define LOCALORREMOTE               0                    //remote还是local模式
#define OccuState                   1                    //根据Occupined Sensor来获取运行状态：有人或无人
#define BMSSignal                   2                    //BMS 启动信号  1---Start  0---OFF
#define FANFault                    3										 //Fan fault

////AO
//#define PTBFan1                     0                    //控制PTB风扇
//#define ERVFan	                    7                    //控制ERV风扇
//#define CHWV                        5                    //控制冷水阀开度
//#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制

////DO
//#define NSLHAlarm                   2                    //报警信息

extern u8 TimeFlag;
extern u8 UnOccupined;
extern u8 ReceiveFlag;
extern u8 TSControl;
extern u8 TempPassive;

typedef enum _startselection
{
  TSstart   = 0x01,             //显示屏启动,正常启动
  BMSStart  = 0x02,	           //BMS启动 
  OccuStart = 0x03,	           //Occupined Sensor 启动模式  
}STARTSEL;

//系统状态标志位
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
u8 OccupanyMode(void);   //有人模式
u8 Unoccupanied(void);   //无人模式

#endif
