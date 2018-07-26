#include "Globle.h"

uint8_t Uart1_SCnt=0;
uint8_t Uart1_Send_buffer[150]={0};
uint8_t Uart1_receive_buffer[150]={0};
uint8_t Uart1_RCnt=0;
uint8_t Uart1_SendBuffSize=0;

uint8_t Uart2_SCnt=0;
uint8_t Uart2_Send_buffer[150]={0};
uint8_t Uart2_receive_buffer[150]={0};
uint8_t Uart2_RCnt=0;
uint8_t Uart2_SendBuffSize=0;

uint8_t Uart3_SCnt=0;
uint8_t Uart3_Send_buffer[150]={0};
uint8_t Uart3_receive_buffer[150]={0};
uint8_t Uart3_RCnt=0;
uint8_t Uart3_SendBuffSize=0;

uint8_t Uart4851_TestFlag=0;
uint8_t Uart4852_TestFlag=0;
uint8_t Wifi_TestFlag=0;

u16 tempData;

//定义全局变量
u8 RS485_RX_BUFF[2048];               //接收缓存区2048字节
u16 RS485_RX_CNT;                   //接收计数器
u8 RS485_FrameFlag;                 //帧结束标记
u8 RS485_TX_BUFF[2048];               //发送缓存区2048字节
u16 RS485_TX_CNT;                   //发送计数器

//Modbus寄存器和单片机寄存器映射关系
vu32 *Modbus_InputIO[100];            //输入开关量寄存器指针(这里使用的是位带操作)
vu32 *Modbus_OutputIO[100];           //输出开关量寄存器指针(这里使用的是位带操作)
u16  *Modbus_HoldReg[1000];            //保持寄存器
