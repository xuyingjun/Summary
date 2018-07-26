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

//用定时器1判断接收空闲时间，当空闲时间大于指定时间，则认为一帧结束
//定时器1中断服务程序   
void TIM2_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志
		TIM_Cmd(TIM2,DISABLE);                       //停止定时器
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

      TIM_ClearITPendingBit(TIM2,TIM_IT_Update);           //清除定时器溢出中断
      TIM_SetCounter(TIM2,0);                   //当接收到一个新的字节时,将定时器1复位为0,重新计时(相当于watchdog)
      TIM_Cmd(TIM2,ENABLE);                    //使能定时器1，开始计时
    }
  }
}

