#ifndef __STM32_CONFIGH_H
#define __STM32_CONFIGH_H

#include "digital_io.h"
#include "systick_timer.h"
#include "adc.h" 
#include "dac.h"
#include "rtd.h"
#include "usart.h"
#include "timer.h"
#include "runtimedata.h"
#include "touchscreen.h"
#include "protocol.h"
#include "logic.h"
#include "globe.h"

//data---电信号值，value---物理意义值
float  TSRegister[12]={0};
float  Registers[16]={0};                 //寄存器值,存储非端口类数据
float  ADC_value[12]={0};                 //ADC物理意义值
float  ADC_data[12]={0};                  //ADC电信号值
float  RTD_value[4]={0};
float  DAC_value[8]={0};
float  DAC_data[8]={0};
u8   	 DI_data[8]={0};
u8     DO_data[8]={0};
float  Setpoint[3]={0};

//全局变量，需要用来与BMS通信
u16 CO2Value=0;
u16 CO2ValueSv=0;
u16 RoomTempPv=0;
u16 RoomTempSetValue=0;
u16 RoomHumPv=0;
u16 RoomHumSetValue=0;
u16 CHWTemp=0;
u16 DPValue=0;
u16 ValveFb=0;
u16 OccSta=0;
u16 ECS=0;


//开闭环配置
//AOAM: analog output auto-manual
//REGAM: register auto-manual
LOOPSEL AOAM[8] = {OPENLOOP,OPENLOOP,OPENLOOP,OPENLOOP,OPENLOOP,OPENLOOP,OPENLOOP,OPENLOOP};
LOOPSEL REGAM[16];
PARAMS AOParams[8] = {{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0}};
//y = a*x +b
PARAMS AIParams[12] = {{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0}};

char ReadPortsBuffer[165];
char WritePortsBuffer[104];
char ReadPIDBuffer[96];
char WritePIDBuffer[97]; //HandleBit(1),PID1---PID4(24/each);

//与上位机通信，功能位初始化代码
//volatile用来修饰被不同线程访问和修改的变量，确保指令不会因为编译器的优化而省略，且要求每次直接读值
unsigned int MCUaddress = 0;                //此MCU的地址，用于唯一标定该MCU，在Config期确定
volatile STAGESEL StageSel = CONFIG;        //通信串口USART1分时复用，阶段选择
volatile TASKSEL TaskSel = NON;			        //Runtime期任务选择 
volatile MODESEL ModeSel = LOAD;		        //Config期模式选择
volatile LINESEL LineSel = ONLINE;
volatile char ReceivedState = 0;
unsigned int  ReceivedNum = 0;
char  ReceivedBuffer[4000];
char  SentBuffer[200];
	
//小板通信定义相关量
char TSReceivedBuffer[24];
char TSReadPortsBuffer[18];
char TSSendBuffer[22]={0};
char TSwriteRegister[12]={0};
char TSSentCheckCode=0;         //待发送数据的效验码
volatile TSTASKSEL TSTaskSel = TSNON;
u8 tx1[24]={0x55,0x66,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x55};
u8 tx2[24]={0x55,0x66,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x10,0x66,0x55};
u8 tx3[24]={0x55,0x66,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x10,0x66,0x55};
u8 tx4[24]={0x55,0x66,0x04,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x10,0x66,0x55};

//系统状态量定义
u8 PannelControl=0;           //面板控制
u8 PannelStatus=0;            //面板状态
u8 DeviceRun=0;               //上位机控制
u8 TSControl=0;               //显示屏控制
u8 BMSControl=0;              //BMS控制
u8 RunStatus=0;               //系统运行状态
u8 TimeFlag=0;                //运行时间标志位
u8 systemStatus=0;            //系统状态
u8 SystemNormal=0;            //系统正常模式
u8 UnOccupined=1;               //系统无人模式
u8 AlarmFlag=0;               //系统报警位
u8 ReceiveFlag=0;             //数据接收完成
u8 TempPassive=1;             //温度Passive模式和Hybrid模式切换标志位：1---Passive Mode，0---Hybrid Mode
volatile STARTSEL StartSel = TSstart;
	
//BMS WriteCommand
u16 PDVComm=0;   //start/stop Command
float RoomHumSv=0.0;
float InletTemp=0.0;
u16 ScheduleComm=0;
u16 IBSCommand=0;

u16 ValveCmd=0;							// 14, 阀紧急全开命令，不执行控制逻辑
u16 ERVCmd=0;							  // 15; ERV紧急全开命令，不执行控制逻辑
u16 ERVState=0;							// 16; ERV状态反馈，0为关，1为全开，其余数值为开度2-100

u8 ForceValveFlag=0;        //强制开阀标志位
u8 ERVFlag=0;               //强制开ERV标志位

u16 PurgeCmd=0;							// 17; Purge开关命令，0为关，1为全开；开持续10分钟
u16 TempSPSet_Unocc=250;				// 18; 无人模式下温度设定值	 
u16 SpareSet1=0;							// 19; 备用参数设置1，0-100
u16 SpareSet2=1;							// 20; 备用参数设置2，0-100
u16 SpareSet3=0;							// 21; 备用参数设置3，0-100

uint16_t HoldReg[16]={0};
	
	
//全局变量
float off_coilTemp=0.0;
float off_coilTempSv=17.0;
float chwInTemp=0.0;
float chwOutTemp=0.0;
float valve=0.0;            //控制阀门开度
float FanSpeed=0.0;         //控制PTB风扇速度
float VavleFb=0.0;
float DiffPressure=0.0;

u16 CO2=0;
u16 CO2Sv=0;
u16 RoomTemp=0;
u16	RoomTempSv=250;
u16 RoomHum=0;
		
//PID参数  Fan Speed Control PID parameters
float Kp;
float Ki;
float Kd;

//valve control PID parameters
float p;
float i;
float d;

//PID control parameters
float error=0;
float derror=0.0;
float last_error=0.0;
float ierror=0.0;
float ValvePosition=0.0;
float VavleFeedback=0.0;

u8 PTBFANRUN=0;
u8 ERVFANRUN=0;

float Para[3]={0.05,0.02,15};

void InitialSys(void);
void RCC_Configuration(void);
void ValueTran(void);
void IWDG_Configuration(void);

#endif
