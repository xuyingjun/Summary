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

//����ȫ�ֱ���
u8 RS485_RX_BUFF[2048];               //���ջ�����2048�ֽ�
u16 RS485_RX_CNT;                   //���ռ�����
u8 RS485_FrameFlag;                 //֡�������
u8 RS485_TX_BUFF[2048];               //���ͻ�����2048�ֽ�
u16 RS485_TX_CNT;                   //���ͼ�����

//Modbus�Ĵ����͵�Ƭ���Ĵ���ӳ���ϵ
vu32 *Modbus_InputIO[100];            //���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
vu32 *Modbus_OutputIO[100];           //����������Ĵ���ָ��(����ʹ�õ���λ������)
u16  *Modbus_HoldReg[1000];            //���ּĴ���
