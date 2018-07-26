#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdio.h>

#define LENGTH  10      //���ݰ�����ͷ+���ݶ�+��β���İ�ͷ����Ϊ 10����λ��ͨ��

typedef enum _stageselection
{
  CONFIG    = 0x01,	    //������-- ��MCU�߼������� -- �������ݴ���FLASH
  RUNTIME   = 0x02,     //����ʱ-- ��MCU�߼������� -- �˿����ݴ���DATABASE
  LUT       = 0x03		  //��������·� -- Look up Table	-- ������֮ǰִ��
}STAGESEL;

typedef enum _taskselection
{
  NON        = 0x00,             //������
  READPORTS  = 0x55,	           //���˿����� -- read ports 
  WRITEPORTS = 0x66,	           //д�˿�����  -- write ports
  READPID    = 0x77,		         //��PID����   -- read pid group
  WRITEPID   = 0x88            	 //дPID����   -- write pid group               
}TASKSEL;

typedef enum _autoselection
{
  MANUAL = 0x10,	  //ģʽѡ��-- ��λ���ֶ�����AO/DO/Registers���
  AUTO   = 0x20       //ģʽѡ��-- ������������Զ�����AO/DO/Registers���
}AUTOSEL;

typedef enum _modeselection
{
  LOAD  = 0x11,	  //ģʽѡ��-- download
  DEBUG = 0x22,	  //ģʽѡ��-- debug model
  RUN   = 0x33,	  //ģʽѡ��-- run model
  STOP  = 0x44    //ģʽѡ��-- stop model
}MODESEL;

typedef enum _loopstateselection
{
  OPENLOOP = 0x00,	        //PID��·״̬ѡ��-- ����
  CLOSELOOP   = 0x01        //PID��·״̬ѡ��-- �ջ�
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
//	���ݰ�ͷ�����(unit: byte)
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
extern unsigned int MCUaddress;                  //��MCU�ĵ�ַ������Ψһ�궨��MCU����Config��ȷ��
extern volatile STAGESEL StageSel;               //ͨ�Ŵ���USART1��ʱ���ã��׶�ѡ��
extern volatile MODESEL ModeSel;			           //Config��ģʽѡ��
extern volatile TASKSEL	TaskSel;			           //Runtime������ѡ�� 
extern volatile LINESEL LineSel;			           //Online Offlien ѡ��
extern volatile char  ReceivedState;					   //���ݴ�������ɱ�־λ
extern unsigned int   ReceivedNum;					     //���յ����ַ��ĸ���
extern char  ReceivedBuffer[4000];			         //���ջ�����
extern char  SentBuffer[200];				             //���ͻ�����

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

   
