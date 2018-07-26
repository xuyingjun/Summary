#include "stm32f10x_it.h"
#include "Globle.h"
#include "stm32f10x_tim.h"
#include "modbus.h"
#include "usart.h"

void TIM2_IRQHandler(void)
{
	//检查指定的TIM中断发生与否，是否发生更新
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			TSDataProcess();	
		}	   
	//清除中断标志位，TIM更新标志位
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}


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
									TSSentDataGeneration(18,TSReadPortsBuffer,SentBuffer);
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
		delay_ms(10);
		DE2=1;
		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);			  	//发送完成 关闭中断
  }
	
}


//用定时器1判断接收空闲时间，当空闲时间大于指定时间，则认为一帧结束
//定时器1中断服务程序   
void TIM1_UP_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM1, TIM_IT_Update)!= RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除中断标志
		TIM_Cmd(TIM1,DISABLE);                       //停止定时器
		DE1=0;                                       //停止接收，切换为发送状态
    RS485_FrameFlag=1;                           //置位帧结束标记符
	}
}

void USART1_IRQHandler(void)
{ 
	u8 res;                                                //存储接收到的数据
  u8 err;                                                //串口硬件错误出发的中断
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)     //接收中断
  {
		if(USART_GetFlagStatus(USART1,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) 
			err=1;//检查到噪音、帧错误或校验错误
    else 
			err=0;
    
		res=USART_ReceiveData(USART1);            //读接收到的字节，同时相关标志自动清除
		if((RS485_RX_CNT<2047)&&(err==0))         //接收到的字节数量，在合理的范围内，同时无其他错误
    {
			RS485_RX_BUFF[RS485_RX_CNT]=res;        //获取数据
      RS485_RX_CNT++;                         //计数

      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);           //清除定时器溢出中断
      TIM_SetCounter(TIM1,0);                   //当接收到一个新的字节时,将定时器1复位为0,重新计时(相当于watchdog)
      TIM_Cmd(TIM1,ENABLE);                    //使能定时器1，开始计时
    }
  }
}

