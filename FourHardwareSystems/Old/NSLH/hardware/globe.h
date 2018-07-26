#ifndef __GLOBE_H
#define __GLOBE_H

#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "rtd.h"
#include "systick_timer.h"	
#include "adc.h" 
#include "dac.h"
#include "digital_io.h"

//IO¿ÚµØÖ·Ó³Éä
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //Êä³ö 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //ÊäÈë 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //Êä³ö 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //ÊäÈë 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //Êä³ö 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //ÊäÈë 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //Êä³ö 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //ÊäÈë 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //Êä³ö 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //ÊäÈë

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //Êä³ö 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //ÊäÈë

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //Êä³ö 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //ÊäÈë


#define USART1_GPIO              GPIOA
#define USART1_CLK               RCC_APB2Periph_USART1
#define USART1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART1_RxPin             GPIO_Pin_10
#define USART1_TxPin             GPIO_Pin_9
#define USART1_IRQn              USART1_IRQn
#define USART1_IRQHandler        USART1_IRQHandler

#define USART2_GPIO              GPIOD
#define USART2_CLK               RCC_APB1Periph_USART2
#define USART2_GPIO_CLK          RCC_APB2Periph_GPIOD
#define USART2_RxPin             GPIO_Pin_6
#define USART2_TxPin             GPIO_Pin_5
#define USART2_IRQn              USART2_IRQn
#define USART2_IRQHandler        USART2_IRQHandler

#define USART3_GPIO              GPIOD
#define USART3_CLK               RCC_APB1Periph_USART3
#define USART3_GPIO_CLK          RCC_APB2Periph_GPIOD
#define USART3_RxPin             GPIO_Pin_9
#define USART3_TxPin             GPIO_Pin_8
#define USART3_IRQn              USART3_IRQn
#define USART3_IRQHandler        USART3_IRQHandler


extern uint8_t Uart1_SCnt;
extern uint8_t Uart1_Send_buffer[150];
extern uint8_t Uart1_receive_buffer[150];
extern uint8_t Uart1_RCnt;
extern uint8_t Uart1_SendBuffSize;

extern uint8_t Uart2_SCnt;
extern uint8_t Uart2_Send_buffer[150];
extern uint8_t Uart2_receive_buffer[150];
extern uint8_t Uart2_RCnt;
extern uint8_t Uart2_SendBuffSize;

extern uint8_t Uart3_SCnt;
extern uint8_t Uart3_Send_buffer[150];
extern uint8_t Uart3_receive_buffer[150];
extern uint8_t Uart3_RCnt;
extern uint8_t Uart3_SendBuffSize;

extern uint8_t Uart4851_TestFlag;
extern uint8_t Uart4852_TestFlag;
extern uint8_t Wifi_TestFlag;

extern uint16_t AdREG[12];                  //modbus Ó³Éä¼Ä´æÆ÷ Ad ch1-ch12
extern uint16_t DiREG[8];                   //modbus Ó³Éä¼Ä´æÆ÷ Di ch1-ch8
extern uint16_t DoREG[8];               //modbus Ó³Éä¼Ä´æÆ÷ D0 ch1-ch8
extern uint16_t DAREG[8];               //modbus Ó³Éä¼Ä´æÆ÷ Da ch1-ch8
extern int16_t  RTDREG[4];                  //modbus Ó³Éä¼Ä´æÆ÷ RTD ch1-ch4

extern uint8_t TimerOverFlag;
extern uint8_t BegainRxFlag;
extern uint8_t BusyFlag;

extern u16 CO2;
extern u16 CO2Sv;
extern u16 RoomTemp;
extern u16 RoomTempSv;
extern u16 RoomHum;
extern float RTDtemperature[4];

//BMS WriteCommand
extern u16 PDVComm;
extern float RoomHumSv;
extern float InletTemp;
extern u16 ScheduleComm;
extern u16 IBSCommand;

extern uint16_t HoldReg[16];

#define modbus_deviceId  0x66
#define RS485MOED_S  GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define RS485MOED_R  GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define DE1 PAout(8)
#define DE2 PDout(7)

extern float ValvePosition;
extern float VavleFeedback;
extern float error;
extern float dataTemp[12];

void DealModBus(void);
u16 crc16(u8 *ptr,u8 len);
void zh_USART_SendData(u8 ch);
void zh_USART_SendString(u8* s,u8 len);
float ValveControl(float SetValue);
#endif
