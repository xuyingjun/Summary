#ifndef __MODBUS_H
#define __MODBUS_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "Globe.h"


//定义全局变量
extern u8 RS485_RX_BUFF[128];               //接收缓存区2048字节
extern u16 RS485_RX_CNT;                   //接收计数器
extern u8 RS485_FrameFlag;                 //帧结束标记
extern u8 RS485_TX_BUFF[128];               //发送缓存区2048字节
extern u16 RS485_TX_CNT;                   //发送计数器

//Modbus寄存器和单片机寄存器映射关系
extern vu32 *Modbus_InputIO[100];            //输入开关量寄存器指针(这里使用的是位带操作)
extern vu32 *Modbus_OutputIO[100];           //输出开关量寄存器指针(这里使用的是位带操作)
extern u16  *Modbus_HoldReg[128];            //保持寄存器

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

extern u16 ValveCmd;							// 14, 阀紧急全开命令，不执行控制逻辑
extern u16 ERVCmd;							  // 15; ERV紧急全开命令，不执行控制逻辑
extern u16 ERVState;							// 16; ERV状态反馈，0为关，1为全开，其余数值为开度2-100

extern u16 PurgeCmd;							// 17; Purge开关命令，0为关，1为全开；开持续10分钟
extern u16 TempSPSet_Unocc;				// 18; 无人模式下温度设定值	 
extern u16 SpareSet1;							// 19; 备用参数设置1，0-100
extern u16 SpareSet2;							// 20; 备用参数设置2，0-100
extern u16 SpareSet3;							// 21; 备用参数设置3，0-100

extern u16 CO2;
extern u16 CO2Sv;
extern u16 RoomTemp;
extern u16	RoomTempSv;
extern u16 RoomHum;

//定义相关函数
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
