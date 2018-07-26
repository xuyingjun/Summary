#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "runtimedata.h"
#include "usart.h"
#include "timer.h"
#include "protocol.h"

volatile UITASKSEL UITaskSel = UINON;	

void TIM1_UP_IRQHandler(void)
{
	static int ScanTime = 3;                           //端口固有扫描周期15s
	//检查指定的TIM中断发生与否，是否发生更新
  if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{
			//RunTime无任务时，每隔15s读取端口数据和PID参数
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
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   
}

void TIM2_UP_IRQHandler(void)
{
	static int ScanTime = 3;                    //小板固有扫描周期30s
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			if(UITaskSel==UINON)
				{
					ScanTime--;
					if(ScanTime==0)
						{
							readPorts(ReadPortsBuffer);
							readPID(ReadPIDBuffer);
							ScanTime=3;
						}
				}
			else
				{
					ScanTime=3;
					UIDataProcess();
				}
		}
}

void USART1_IRQHandler(void)
{
	static unsigned int receivedCount = 0;
  char receivedData;
  char highByte,lowByte;
  char flag = 0;                     //the status flag of completion of receiving data
  int  isTrue = 0;	                  // Check Code is right?
  int i;
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE))
		{
			receivedData = Usart_GetChar(USART1);
			USART_ClearITPendingBit(USART1, USART_FLAG_ORE);//清除过流中断标志位
		}
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
		{
			receivedData = Usart_GetChar(USART1); 
			ReceivedBuffer[receivedCount++] = receivedData;
			if(receivedCount >= 2)
				{
					highByte = ReceivedBuffer[receivedCount-2];
					lowByte =  ReceivedBuffer[receivedCount-1];
					if((highByte & lowByte & 0xff) == 0xff)
						flag = 1;
				}
			if(flag)
				{
					flag = 0;
					ReceivedNum = receivedCount;	
					receivedCount = 0;  
					isTrue = CheckReceivedData(ReceivedBuffer);
					if(isTrue)
						{
							PacketHeaderAnalysis(ReceivedBuffer);
							//if(StageSel == CONFIG)
								//{
									//if(ModeSel == LOAD)
										//FlashPageWrite(0,ReceivedBuffer,ReceivedNum);
									//else if(ModeSel == RUN || ModeSel == DEBUG)
										//ReceivedState = 0x01;        //允许进入链表主程序
								//}
							//else 
								if(StageSel == RUNTIME)
								{
									if(TaskSel == READPORTS)
										{
											SentDataGeneration(165,ReadPortsBuffer,SentBuffer);
											UsartSendData(LENGTH+165+2,SentBuffer);
										}
									else if(TaskSel == READPID)
										{
											SentDataGeneration(96,ReadPIDBuffer,SentBuffer);		  
											UsartSendData(LENGTH+96+2,SentBuffer);
										}
									else if(TaskSel == WRITEPORTS)
										{
											UsartSendData(ReceivedNum,ReceivedBuffer);
											for(i=0;i<104;i++)
												WritePortsBuffer[i] = ReceivedBuffer[i+LENGTH];
										}
									else if(TaskSel == WRITEPID)
										{
											UsartSendData(ReceivedNum,ReceivedBuffer);
											for(i=0;i<97;i++)
												WritePIDBuffer[i] = ReceivedBuffer[i+LENGTH];
										}
								}
							}		
						}	
					}
 //清除中断标志位
 USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
}

void USART2_IRQHandler(void)
{
	//static unsigned int receivedCount = 0;
	
}



