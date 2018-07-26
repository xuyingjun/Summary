#include <stdio.h>
#include "stm32f10x.h"
#include "systick_timer.h"	

/*微秒级精确延时函数*/
void delay_us(__IO uint32_t nTime)	                 //延时多少微秒，n就输入多少！
{
  SysTick->LOAD=72*nTime;		                         //装载计数值，因为时钟72M，72次在1μs
  SysTick->CTRL=0x00000005;                          //时钟来源设为为HCLK(72M)，打开定时器
  while(!(SysTick->CTRL&0x00010000));                //等待计数到0
  SysTick->CTRL=0x00000004;                          //关闭定时器
}

/*毫秒级精确延时函数*/
void delay_ms(__IO uint32_t nTime)	                 //延时多少微秒，n就输入多少！
{
  int cnt = 1000;
  SysTick->LOAD=72*nTime;		                         //装载计数值，因为时钟72M，72次在1μs
  SysTick->CTRL=0x00000005; 				                 //时钟来源设为为HCLK(72M)，打开定时器
  while(1)
		{                 
			while(!(SysTick->CTRL&0x00010000));              //等待计数到0
			SysTick->CTRL &= ~0x00010000;
			cnt--;
			if(cnt == 0) break;
		}
  SysTick->CTRL=0x00000004;                          //关闭定时器
}

/*秒级精确延时函数*/
void delay_s(__IO uint32_t nTime)	                   //延时多少秒，n就输入多少！
{
  int cnt = 1000;
  SysTick->LOAD=72*1000*nTime;		                   //装载计数值，因为时钟72M，72次在1μs
  SysTick->CTRL=0x00000005; 				                 //时钟来源设为为HCLK(72M)，打开定时器
  while(1) 
		{                 
			while(!(SysTick->CTRL&0x00010000));              //等待计数到0
			SysTick->CTRL &= ~0x00010000;
			cnt--;
			if(cnt == 0) break;
		}
  SysTick->CTRL=0x00000004;                         //关闭定时器
}

/********************Method Two For Use Of SysTick Timer**********************/	
/*					
#define SYSTICK_FREQUENCY_HZ       1000
static __IO uint32_t uwTimingDelay;
static void TimingDelay_Decrement(void);
void SysTick_Handler(void);
*/
/*
void SysTick_init()
{
  // Use SysTick as reference for the timer 
  SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY_HZ);		
}
 */
/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in SysTick ticks.
 * @retval None
 */
 /*
void Delay_ms(__IO uint32_t nTime)
{
  uwTimingDelay = nTime;
  while (uwTimingDelay != 0);
}

void Delay_s(__IO uint32_t nTime)
{
  uwTimingDelay = nTime*1000;
  while (uwTimingDelay != 0);
}
*/
/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
 /*
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
    {
      uwTimingDelay--;
    }
}
*/
/**
 * @brief  This function is the SysTick Handler.
 * @param  None
 * @retval None
 */
 /*
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}
*/

