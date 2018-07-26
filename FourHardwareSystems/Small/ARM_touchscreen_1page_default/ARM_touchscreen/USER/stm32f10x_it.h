/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

#include "stm32f10_usart.h"
/* Exported types ------------------------------------------------------------*/
#define CLENGTH  53
#define CSTART 0
#define CSTOP  50

#define ULENGTH 22
#define USTART 3
#define USTOP 21

static u8 timercnt=0  ;

typedef struct Frame
{
	u8 num;
    u8 format;
	u8 address; 
	u8 pattern;
	u8 autom;
}frame;

typedef struct _Frame
{
	u8 num;
    u8 format;
	u8 pattern;

}disframe;

u8   Cntnum=0;

u8 Send_read[24]={0x55,0x66,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,0x66,0x55};
u8 Send_set[24]={0x55,0x66,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0x66,0x55};
u8 Send_start[24]={0x55,0x66,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,36,0x66,0x55};
u8 Send_stop[24]={0x55,0x66,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,36,0x66,0x55};

u8 Send_Emstart[24]={0x55,0x66,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,36,0x66,0x55};
u8 Send_Emstop[24] ={0x55,0x66,6,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,36,0x66,0x55};

extern u8 buffercnt0=0;
extern u8 TDorD1=0, TDorD2=0, pageflag0=0;
extern u8 return_flag=0;
extern u8 kflag0=0;


 u8 timer1flag=0;
 u8 nCount;
 u8 mCount;

extern u8 indata_type = 0;

extern u8 PAGEflag;
extern unsigned char getdataflag;

extern	void dataProcess(void);
extern  void Send_finish(unsigned char a);
extern  void showonoff(int onoff) 	;

u8 usart2_buff[24];
u8 usart_r_buff[24];
extern u8 tdata[4];
extern void ReadDHT11(void);
u8 format=0;

//extern u8 Uart2_SCnt;
//extern u8 Uart2_Send_buffer[80];
//extern u8 Uart2_receive_buffer[80];
//extern u8 Uart2_RCnt;

int Uart2_SendBuffSize;
void packagedata(u16 PvRH,u16 PvTe,u16 SvTe,u16 PvCO2,u16 SvCO2,u8 flag) ;

int usart1_buff[24];	
//extern u8 CRC_check(u8 arr[],u8 start, u8 stop);

long int cntValue  = 0;

int IC2Value  = 0;
int IC1Value   =0;
extern int IC1sum[50]	;
extern int IC2sum[50]	;
extern u8 lcdBG ;
extern u8 lcdBG_timecount ;




extern u8 Oflag 	  ;
extern u8 Tflag 	  ;
  
u8  co2flag=0;
u8  colcnt =0;
u8  Capflag=0;

extern void Tim2SP(void);

void usart2_senden(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void UART1_IRQHandler(void);
void TIM1_UP_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);
void TIM6_IRQHandler(void);
void usart_senddata(u8 arr[]);
void usart2_senddata(u8 typeA);
void usart2_senddata2(u8 arr[]);
void Receiver_process(void);




#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
