#ifndef __TOUCHSCREEN_H
#define __TOUCHSCREEN_H

#include <stdio.h>
#include "stm32f10x.h"

#define TSLENGTH 3      //UI数据包(包头+数据段+包尾),包头的长度是3，  大板和小板之间的通信

typedef enum _tstaskselection
{
  TSNON      = 0x00,             //无任务
  READDATA   = 0x01,	           //读数据
  SETDATA    = 0x02,	           //设置数据
  START      = 0x03,		         //启动
  TSSTOP     = 0x04              //停止
}TSTASKSEL;

extern volatile TSTASKSEL TSTaskSel;

extern char TSReadPortsBuffer[18];

extern char tx1[24]; 
extern char tx2[24]; 
extern char tx3[24]; 
extern char tx4[24]; 

extern u8 localControl;
extern char  TSReceivedBuffer[24];

extern float CO2;
extern float CO2Sv;
extern float RoomTemp;
extern float RoomTempSv;
extern float RoomHum;

void TSDataProcess(void);
void TSreadPorts(char* readPortsBuffer);
void TSSentDataGeneration(int length,char* dataBuffer,char* sentBuffer);
int CheckTSReceivedData(char* receivedBuffer);
int TSPacketAnalysis(char * data);
void GetTouchScreenValue(void);

#endif
