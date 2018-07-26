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


//定义全局变量
extern u8 RS485_RX_BUFF[128];               //接收缓存区2048字节
extern u16 RS485_RX_CNT;                   //接收计数器
extern u8 RS485_FrameFlag;                 //帧结束标记
extern u8 RS485_TX_BUFF[128];               //发送缓存区2048字节
extern u16 RS485_TX_CNT;                   //发送计数器

//Modbus寄存器和单片机寄存器映射关系
extern vu32 *Modbus_InputIO[100];            //输入开关量寄存器指针(这里使用的是位带操作)
extern vu32 *Modbus_OutputIO[100];           //输出开关量寄存器指针(这里使用的是位带操作)
extern u16  *Modbus_HoldReg[128];            //保持寄存器


///////////////////////////////////////////////////////////////////////////////////////
//用定时器1判断接收空闲时间，当空闲时间大于指定时间，则认为一帧结束
//定时器1中断服务程序   
void TIM1_UP_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除中断标志
		TIM_Cmd(TIM1,DISABLE);       //停止定时器
		DE1=0;//停止接收，切换为发送状态
    RS485_FrameFlag=1;//置位帧结束标记符
	}
}

void USART1_IRQHandler(void)
{ 
	u8 res;
  u8 err;
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
  {
		if(USART_GetFlagStatus(USART1,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//检查到噪音、帧错误或校验错误
      else err=0;
     res=USART_ReceiveData(USART1); //读接收到的字节，同时相关标志自动清除
		if((RS485_RX_CNT<2047)&&(err==0))
    {
			RS485_RX_BUFF[RS485_RX_CNT]=res;
      RS485_RX_CNT++;

      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除定时器溢出中断
      TIM_SetCounter(TIM1,0);//当接收到一个新的字节时,将定时器1复位为0,重新计时(相当于watchdog)
      TIM_Cmd(TIM1,ENABLE);//使能定时器1，开始计时
     }
   }
}

void TIM2_IRQHandler(void)
{
	static int ScanTime = 3;                           //端口固有扫描周期3s
	//检查指定的TIM中断发生与否，是否发生更新
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			//RunTime无任务时，每隔15s读取端口数据和PID参数
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
				//如果有任务，就进行对应的任务操作
				ScanTime = 3;
				TSDataProcess();	
			}
		}	   
	//清除中断标志位，TIM更新标志位
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}

//void USART2_IRQHandler(void)
//{
//	char flag=0;
//	int IsTrue=0;
//	static unsigned int receivedCount=0;
//	char receivedData;
//	
//	//	//判断串口是否发生指定中断，是否发生溢出错误中断
//	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE))
//		{
//			receivedData = Usart_GetChar(USART2);
//			USART_ClearITPendingBit(USART2, USART_FLAG_ORE);//清除相应的标志位
//		}
//		
//	if(USART_GetITStatus(USART2, USART_IT_RXNE))
//	{
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//		receivedData = Usart_GetChar(USART2); 
//		TSReceivedBuffer[receivedCount++] = receivedData;
//			//接收到数据，最少2个字节
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
	
	//判断串口是否发生指定中断，是否发生溢出错误中断
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE))
		{
			receivedData = Usart_GetChar(USART2);
			USART_ClearITPendingBit(USART2, USART_FLAG_ORE);//清除相应的标志位
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
		
	//串口发生接收中断			
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
		{
			 //清除中断标志位
			USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
			receivedData = Usart_GetChar(USART2); 
			TSReceivedBuffer[receivedCount++] = receivedData;
			//接收到数据，最少2个字节
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
					//判断接收数据校验位是否正确					
					isTrue = CheckTSReceivedData(TSReceivedBuffer);
					if(isTrue)
						{
							//解析接收到数据
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
		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);			  	//发送完成 关闭中断
  }
}

//void TIM3_IRQHandler(void)
//{
//	int count=0;                           
//	//检查指定的TIM中断发生与否，是否发生更新
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
//	//清除中断标志位，TIM更新标志位
//	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
//}

void TIM3_IRQHandler(void)
{
	static int ScanTime = 3;                           //端口固有扫描周期3s
	//检查指定的TIM中断发生与否，是否发生更新
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
			//RunTime无任务时，每隔6s读取端口数据和PID参数
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
				//如果有任务，就进行对应的任务操作
				ScanTime = 3;
				DataProcess();	
			}
		}	   
	//清除中断标志位，TIM更新标志位
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
