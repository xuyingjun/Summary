#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "runtimedata.h"
#include "systick_timer.h"
#include "usart.h"
#include "timer.h"
#include "protocol.h"
#include "touchscreen.h"
#include "globe.h"
#include "string.h"
#include "ChangingPara.h"
#include "modbus.h"				//Yan


extern u8 TimeFlag;


//����ȫ�ֱ���
extern u8 RS485_RX_BUFF[128];               //���ջ�����2048�ֽ�
extern u16 RS485_RX_CNT;                   //���ռ�����
extern u8 RS485_FrameFlag;                 //֡�������
extern u8 RS485_TX_BUFF[128];               //���ͻ�����2048�ֽ�
extern u16 RS485_TX_CNT;                   //���ͼ�����

//Modbus�Ĵ����͵�Ƭ���Ĵ���ӳ���ϵ
extern vu32 *Modbus_InputIO[100];            //���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
extern vu32 *Modbus_OutputIO[100];           //����������Ĵ���ָ��(����ʹ�õ���λ������)
extern u16  *Modbus_HoldReg[128];            //���ּĴ���


///////////////////////////////////////////////////////////////////////////////////////
//�ö�ʱ��1�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬����Ϊһ֡����
//��ʱ��1�жϷ������   
void TIM1_UP_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //����жϱ�־
		TIM_Cmd(TIM1,DISABLE);       //ֹͣ��ʱ��
		DE1=0;//ֹͣ���գ��л�Ϊ����״̬
    RS485_FrameFlag=1;//��λ֡������Ƿ�
	}
}

void USART1_IRQHandler(void)
{ 
	u8 res;
  u8 err;
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
  {
		if(USART_GetFlagStatus(USART1,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��鵽������֡�����У�����
      else err=0;
     res=USART_ReceiveData(USART1); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
		if((RS485_RX_CNT<2047)&&(err==0))
    {
			RS485_RX_BUFF[RS485_RX_CNT]=res;
      RS485_RX_CNT++;

      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//�����ʱ������ж�
      TIM_SetCounter(TIM1,0);//�����յ�һ���µ��ֽ�ʱ,����ʱ��1��λΪ0,���¼�ʱ(�൱��watchdog)
      TIM_Cmd(TIM1,ENABLE);//ʹ�ܶ�ʱ��1����ʼ��ʱ
     }
   }
}

void TIM2_IRQHandler(void)
{
	static int ScanTime = 3;                           //�˿ڹ���ɨ������3s
	//���ָ����TIM�жϷ�������Ƿ�������
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			//RunTime������ʱ��ÿ��15s��ȡ�˿����ݺ�PID����
			if(TSTaskSel == NON)
				{
					ScanTime--;
					if(ScanTime == 0)
						{
							TSreadPorts(TSReadPortsBuffer);
							ScanTime = 3;
						}
				}	
			else
			{
				//��������񣬾ͽ��ж�Ӧ���������
				ScanTime = 3;
				TSDataProcess();	
			}
		}	   
	//����жϱ�־λ��TIM���±�־λ
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}

//void USART2_IRQHandler(void)
//{
//	char flag=0;
//	int IsTrue=0;
//	static unsigned int receivedCount=0;
//	char receivedData;
//	
//	//	//�жϴ����Ƿ���ָ���жϣ��Ƿ�����������ж�
//	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE))
//		{
//			receivedData = Usart_GetChar(USART2);
//			USART_ClearITPendingBit(USART2, USART_FLAG_ORE);//�����Ӧ�ı�־λ
//		}
//		
//	if(USART_GetITStatus(USART2, USART_IT_RXNE))
//	{
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//		receivedData = Usart_GetChar(USART2); 
//		TSReceivedBuffer[receivedCount++] = receivedData;
//			//���յ����ݣ�����2���ֽ�
//		if(receivedCount == 24)
//		{
//			if((TSReceivedBuffer[22]==0x66)&&(TSReceivedBuffer[23]==0x55))
//			{
//				flag=1;
//			}
//		}
//		if(flag)
//		{
//			ReceivedNum = receivedCount;	
//			receivedCount=0;
//			IsTrue = CheckTSReceivedData(TSReceivedBuffer);
//			if(IsTrue)
//			{
//				TSPacketAnalysis(TSReceivedBuffer);
//				if(TSTaskSel == READDATA)
//				{
//					TSSentDataGeneration(18,TSReadPortsBuffer,SentBuffer);
//				}
//				else if(TSTaskSel == SETDATA)
//				{
//					DE2=0;
//					Uart2_SCnt=0;
//					Uart2_SendBuffSize=24;
//					memcpy(Uart2_Send_buffer,tx2,24);
//					USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//				}
//				else if(TSTaskSel == START)
//				{
//					DE2=0;
//					TSControl=1;
//					Uart2_SCnt=0;
//					Uart2_SendBuffSize=24;
//					memcpy(Uart2_Send_buffer,tx3,24);
//					USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//				}
//				else if(TSTaskSel == TSSTOP)
//				{
//					DE2=0;
//					TSControl=0;
//					Uart2_SCnt=0;
//					Uart2_SendBuffSize=24;
//					memcpy(Uart2_Send_buffer,tx4,24);
//					USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//				}
//			}
//		}
//		
//		else
//		{
//			Uart2_RCnt=0;
//		}
//	}
//	
//	if(USART_GetITStatus(USART2, USART_IT_TXE))
//	{
//		USART_ClearITPendingBit(USART2, USART_IT_TXE);
//		if(Uart2_SCnt<Uart2_SendBuffSize)
//		{
//			USART_SendData(USART2,Uart2_Send_buffer[Uart2_SCnt++]);
//		}
//		else
//		{
//			Uart2_SCnt=0;
//			DE2=1;
//			USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
//		}
//	}
//	
//}


void USART2_IRQHandler(void)
{
	static unsigned int receivedCount = 0;
  char receivedData;
  char flag = 0;                     //the status flag of completion of receiving data
  int  isTrue = 0;	                  // Check Code is right?
	
	//�жϴ����Ƿ���ָ���жϣ��Ƿ�����������ж�
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE))
		{
			receivedData = Usart_GetChar(USART2);
			USART_ClearITPendingBit(USART2, USART_FLAG_ORE);//�����Ӧ�ı�־λ
		}
		
		if(USART_GetFlagStatus(USART2,USART_FLAG_NE)!=RESET)
		{
			USART_ClearFlag(USART2,USART_FLAG_NE);
		}
		
		if(USART_GetFlagStatus(USART2,USART_FLAG_FE)!=RESET)
		{
			USART_ClearFlag(USART2,USART_FLAG_FE);
		}
		
		if(USART_GetFlagStatus(USART2,USART_FLAG_PE)!=RESET)
		{
			USART_ClearFlag(USART2,USART_FLAG_PE);
		}
		
	//���ڷ��������ж�			
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
		{
			 //����жϱ�־λ
			USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
			receivedData = Usart_GetChar(USART2); 
			TSReceivedBuffer[receivedCount++] = receivedData;
			//���յ����ݣ�����2���ֽ�
			if(receivedCount == 24)
				{
					if((TSReceivedBuffer[22]==0x66)&&(TSReceivedBuffer[23]==0x55))
						flag = 1;
				}
			if(flag)
				{
					flag = 0;
					ReceivedNum = receivedCount;	
					receivedCount = 0;
					//�жϽ�������У��λ�Ƿ���ȷ					
					isTrue = CheckTSReceivedData(TSReceivedBuffer);
					if(isTrue)
						{
							//�������յ�����
							TSPacketAnalysis(TSReceivedBuffer);
							if(TSTaskSel == READDATA)
								{
									TSSentDataGeneration(16,TSReadPortsBuffer,SentBuffer);
									DE2=0;
									Usart2SendData(24,(char*)SentBuffer);
								}
							else if(TSTaskSel == SETDATA)
								{
									Usart2SendData(24,(char*)tx2);
								}
							else if(TSTaskSel == START)
								{
									TSControl=1;
									Usart2SendData(24,(char*)tx3);
								}
							else if(TSTaskSel == TSSTOP)
								{
									TSControl=0;
									Usart2SendData(24,(char*)tx4);
								}
						}		
				}		
		}
		
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {
		USART_ClearITPendingBit(USART2,USART_IT_TXE);
		Delay_ms(10);
		DE2=1;
		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);			  	//������� �ر��ж�
  }
}

//void TIM3_IRQHandler(void)
//{
//	int count=0;                           
//	//���ָ����TIM�жϷ�������Ƿ�������
//  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//		{
//			TimeFlag=0;
//			count++;
//			if(count==600)
//				{
//					count=0;
//					TimeFlag=1;
//				}
//				
//		}	   
//	//����жϱ�־λ��TIM���±�־λ
//	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
//}

void TIM3_IRQHandler(void)
{
	static int ScanTime = 3;                           //�˿ڹ���ɨ������3s
	//���ָ����TIM�жϷ�������Ƿ�������
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
			//RunTime������ʱ��ÿ��6s��ȡ�˿����ݺ�PID����
			if(TaskSel == NON)
				{
					ScanTime--;
					if(ScanTime == 0)
						{
							readPorts(ReadPortsBuffer);
							readPID(ReadPIDBuffer);
							ScanTime = 3;
						}
				}	
			else
			{
				//��������񣬾ͽ��ж�Ӧ���������
				ScanTime = 3;
				DataProcess();	
			}
		}	   
	//����жϱ�־λ��TIM���±�־λ
	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);   
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		ValvePosition = ValveControl(RoomTempSv);
		if(OccSta==0) ValvePosition=ValveControl(TempSPSet_Unocc);
	}
}
