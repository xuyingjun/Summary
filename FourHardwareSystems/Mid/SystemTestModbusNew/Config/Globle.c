#include "Globle.h"

uint16_t ADC_ConvertedValue[10][6];

uint8_t Uart1_SCnt=0;
uint8_t Uart1_Send_buffer[150]={0};
uint8_t Uart1_receive_buffer[150]={0};
uint8_t Uart1_RCnt=0;
uint8_t Uart1_SendBuffSize=0;

//AI端口数据
float advalue[7]={0};

float temperature=0.0;

//modbus
uint8_t ReadReg[100]={0};
uint16_t WriteReg[100]={0};
uint8_t TimerOverFlag=0;
uint8_t BegainRxFlag=0;
uint8_t BusyFlag=0;


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


u16 tempData=0;
