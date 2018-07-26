#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdio.h>

#define LENGTH  10      //数据包（包头+数据段+包尾）的包头长度为 10，上位机通信

typedef enum _stageselection
{
  CONFIG    = 0x01,	    //配置期-- 即MCU逻辑配置期 -- 配置数据存于FLASH
  RUNTIME   = 0x02,     //运行时-- 即MCU逻辑运行期 -- 端口数据存于DATABASE
  LUT       = 0x03		  //查表数据下发 -- Look up Table	-- 配置期之前执行
}STAGESEL;

typedef enum _taskselection
{
  NON        = 0x00,             //无任务
  READPORTS  = 0x55,	           //读端口任务 -- read ports 
  WRITEPORTS = 0x66,	           //写端口任务  -- write ports
  READPID    = 0x77,		         //读PID任务   -- read pid group
  WRITEPID   = 0x88            	 //写PID任务   -- write pid group               
}TASKSEL;

typedef enum _autoselection
{
  MANUAL = 0x10,	  //模式选择-- 上位机手动控制AO/DO/Registers输出
  AUTO   = 0x20       //模式选择-- 程序根据运行自动控制AO/DO/Registers输出
}AUTOSEL;

typedef enum _modeselection
{
  LOAD  = 0x11,	  //模式选择-- download
  DEBUG = 0x22,	  //模式选择-- debug model
  RUN   = 0x33,	  //模式选择-- run model
  STOP  = 0x44    //模式选择-- stop model
}MODESEL;

typedef enum _loopstateselection
{
  OPENLOOP = 0x00,	        //PID环路状态选择-- 开环
  CLOSELOOP   = 0x01        //PID环路状态选择-- 闭环
}LOOPSEL;

typedef enum _onoffline
{
  ONLINE = 0x00,
  OFFLINE = 0x01
}LINESEL;

typedef struct _params		//y = a*x +b
{
  float a;
  float b;
}PARAMS;


///////////////////////////////////////////////
//	数据包头的组成(unit: byte)
//	address1,
//  address2,
//  address3,
//  address4,
//	stagesel,
//  functionsel,
//  checkcode,
//  length_h,
//  length_l,
//  reserved
//////////////////////////////////////////////
extern unsigned int MCUaddress;                  //此MCU的地址，用于唯一标定该MCU，在Config期确定
extern volatile STAGESEL StageSel;               //通信串口USART1分时复用，阶段选择
extern volatile MODESEL ModeSel;			           //Config期模式选择
extern volatile TASKSEL	TaskSel;			           //Runtime期任务选择 
extern volatile LINESEL LineSel;			           //Online Offlien 选择
extern volatile char  ReceivedState;					   //数据串接收完成标志位
extern unsigned int   ReceivedNum;					     //接收到的字符的个数
extern char  ReceivedBuffer[4000];			         //接收缓存区
extern char  SentBuffer[200];				             //发送缓存区

int PacketHeaderAnalysis(char * data);
void PacketHeaderState(char * data);
void SentDataGeneration(int length,char* dataBuffer,char* sentBuffer);
int  CheckReceivedData(char* receivedBuffer);
char checkSentData(int length,char* dataBuffer);
char getIntHighByte(float data);
char getIntLowByte(float data);
char getDecHighByte(float data);
char getDecLowByte(float data);
#endif /*PROTOCOL_H*/

   
