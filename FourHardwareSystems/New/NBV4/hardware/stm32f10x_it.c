#include "stm32f10x_it.h"
#include "Globle.h"
#include "stm32f10x_tim.h"
#include "modbus.h"
#include "usart.h"

void TIM2_IRQHandler(void)
{
	//���ָ����TIM�жϷ�������Ƿ�������
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			TSDataProcess();	
		}	   
	//����жϱ�־λ��TIM���±�־λ
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}


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
		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);			  	//������� �ر��ж�
  }
	
}


//�ö�ʱ��1�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬����Ϊһ֡����
//��ʱ��1�жϷ������   
void TIM1_UP_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM1, TIM_IT_Update)!= RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //����жϱ�־
		TIM_Cmd(TIM1,DISABLE);                       //ֹͣ��ʱ��
		DE1=0;                                       //ֹͣ���գ��л�Ϊ����״̬
    RS485_FrameFlag=1;                           //��λ֡������Ƿ�
	}
}

void USART1_IRQHandler(void)
{ 
	u8 res;                                                //�洢���յ�������
  u8 err;                                                //����Ӳ������������ж�
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)     //�����ж�
  {
		if(USART_GetFlagStatus(USART1,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) 
			err=1;//��鵽������֡�����У�����
    else 
			err=0;
    
		res=USART_ReceiveData(USART1);            //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
		if((RS485_RX_CNT<2047)&&(err==0))         //���յ����ֽ��������ں���ķ�Χ�ڣ�ͬʱ����������
    {
			RS485_RX_BUFF[RS485_RX_CNT]=res;        //��ȡ����
      RS485_RX_CNT++;                         //����

      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);           //�����ʱ������ж�
      TIM_SetCounter(TIM1,0);                   //�����յ�һ���µ��ֽ�ʱ,����ʱ��1��λΪ0,���¼�ʱ(�൱��watchdog)
      TIM_Cmd(TIM1,ENABLE);                    //ʹ�ܶ�ʱ��1����ʼ��ʱ
    }
  }
}

