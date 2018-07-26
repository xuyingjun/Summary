#ifndef _GLOBLE_H
#define _GLOBLE_H

#include "stm32f10x_it.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "rtd.h"
#include "ads1115.h"
#include "systick_timer.h"	
#include "DAC8554.h"
#include "MuxCfg.h"
#include "Dio.h"
#include "Pwm.h"

//IO口地址映射
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

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


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

#define DE1 PAout(8)
#define DE2 PDout(7)
#define LED PBout(9)

extern u16 tempData;

//定义全局变量
extern u8 RS485_RX_BUFF[2048];               //接收缓存区2048字节
extern u16 RS485_RX_CNT;                   //接收计数器
extern u8 RS485_FrameFlag;                 //帧结束标记
extern u8 RS485_TX_BUFF[2048];               //发送缓存区2048字节
extern u16 RS485_TX_CNT;                   //发送计数器

//Modbus寄存器和单片机寄存器映射关系
extern vu32 *Modbus_InputIO[100];            //输入开关量寄存器指针(这里使用的是位带操作)
extern vu32 *Modbus_OutputIO[100];           //输出开关量寄存器指针(这里使用的是位带操作)
extern u16  *Modbus_HoldReg[1000];            //保持寄存器


#endif

