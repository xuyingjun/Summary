#include "stm32f10x_it.h"
#include "Globle.h"
#include "modbus.h"

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//�ö�ʱ��1�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬����Ϊһ֡����
//��ʱ��1�жϷ������   
void TIM2_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //����жϱ�־
		TIM_Cmd(TIM2,DISABLE);                       //ֹͣ��ʱ��
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

      TIM_ClearITPendingBit(TIM2,TIM_IT_Update);           //�����ʱ������ж�
      TIM_SetCounter(TIM2,0);                   //�����յ�һ���µ��ֽ�ʱ,����ʱ��1��λΪ0,���¼�ʱ(�൱��watchdog)
      TIM_Cmd(TIM2,ENABLE);                    //ʹ�ܶ�ʱ��1����ʼ��ʱ
    }
  }
}

