#ifndef _STM32F10X_CONFIG_H
#define _STM32F10X_CONFIG_H
	
#include "stm32f10_usart.h" 
#include "stm32f10_gpio.h" 

#include <string.h>	  

//****************************************************
  typedef struct  
{	
	u8	bus16;				    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8	wramcmd;		//��ʼдgramָ��
	u8  setxcmd;		//����x����ָ��
	u8  setycmd;		//����y����ָ��	 
}_lcd_dev; 


extern u8 Send_read[24];
extern u8 Send_set[24];
extern u8 Send_start[24];
extern u8 Send_stop[24];

extern void usart2_senddata(u8 typeA);
extern void packagedata(u16 PvRH,u16 PvTe,u16 SvTe,u16 PvCO2,u16 SvCO2,u8 flag) ;
extern void Tim1ST(void);
extern void Tim1SP(void);
extern u8  timer1flag ;
//LCD����

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern int IC1sum[20];
extern int IC2sum[20];

extern u8 tdata[4];
extern void ReadDHT11(void);
/***********************************************************/ 

void RCC_Configuration(void);
void Send_finish(unsigned char a);
void showonoff(int onoff) ;
void dataProcess(void);

void some_Configuration(void);
void IWDG_Configuration(void);
#endif
