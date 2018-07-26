#ifndef __TOUCHSCREEN_H
#define __TOUCHSCREEN_H

#include <stdio.h>
#include "stm32f10x.h"

#define TSLENGTH 3      //UI���ݰ�(��ͷ+���ݶ�+��β),��ͷ�ĳ�����3��  ����С��֮���ͨ��

typedef enum _tstaskselection
{
  TSNON      = 0x00,             //������
  READDATA   = 0x01,	           //������
  SETDATA    = 0x02,	           //��������
  START      = 0x03,		         //����
  TSSTOP     = 0x04              //ֹͣ
}TSTASKSEL;

extern volatile TSTASKSEL TSTaskSel;

extern char TSReadPortsBuffer[18];
extern char TSReceivedBuffer[24];
extern u8 TSControl;

extern u16 CO2;
extern u16 CO2Sv;
extern u16 RoomTemp;
extern u16 RoomTempSv;
extern u16 RoomHum;

extern u8 tx1[24]; 
extern u8 tx2[24]; 
extern u8 tx3[24]; 
extern u8 tx4[24]; 

void TSDataProcess(void);
void TSreadPorts(char* readPortsBuffer);
void TSSentDataGeneration(int length,char* dataBuffer,char* sentBuffer);
int CheckTSReceivedData(char* receivedBuffer);
int TSPacketAnalysis(char * data);
void GetTouchScreenValue(void);

#endif
