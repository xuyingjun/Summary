#include "Globle.h"

uint16_t ADC_ConvertedValue[10][6];

uint8_t Uart1_SCnt=0;
uint8_t Uart1_Send_buffer[150]={0};
uint8_t Uart1_receive_buffer[150]={0};
uint8_t Uart1_RCnt=0;
uint8_t Uart1_SendBuffSize=0;

//AI�˿�����
float advalue[7]={0};

float temperature=0.0;

//modbus
uint8_t ReadReg[100]={0};
uint16_t WriteReg[100]={0};
uint8_t TimerOverFlag=0;
uint8_t BegainRxFlag=0;
uint8_t BusyFlag=0;


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


u16 tempData=0;
