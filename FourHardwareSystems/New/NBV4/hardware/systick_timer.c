#include <stdio.h>
#include "stm32f10x.h"
#include "systick_timer.h"	

/*΢�뼶��ȷ��ʱ����*/
void delay_us(__IO uint32_t nTime)	                 //��ʱ����΢�룬n��������٣�
{
  SysTick->LOAD=72*nTime;		                         //װ�ؼ���ֵ����Ϊʱ��72M��72����1��s
  SysTick->CTRL=0x00000005;                          //ʱ����Դ��ΪΪHCLK(72M)���򿪶�ʱ��
  while(!(SysTick->CTRL&0x00010000));                //�ȴ�������0
  SysTick->CTRL=0x00000004;                          //�رն�ʱ��
}

/*���뼶��ȷ��ʱ����*/
void delay_ms(__IO uint32_t nTime)	                 //��ʱ����΢�룬n��������٣�
{
  int cnt = 1000;
  SysTick->LOAD=72*nTime;		                         //װ�ؼ���ֵ����Ϊʱ��72M��72����1��s
  SysTick->CTRL=0x00000005; 				                 //ʱ����Դ��ΪΪHCLK(72M)���򿪶�ʱ��
  while(1)
		{                 
			while(!(SysTick->CTRL&0x00010000));              //�ȴ�������0
			SysTick->CTRL &= ~0x00010000;
			cnt--;
			if(cnt == 0) break;
		}
  SysTick->CTRL=0x00000004;                          //�رն�ʱ��
}

/*�뼶��ȷ��ʱ����*/
void delay_s(__IO uint32_t nTime)	                   //��ʱ�����룬n��������٣�
{
  int cnt = 1000;
  SysTick->LOAD=72*1000*nTime;		                   //װ�ؼ���ֵ����Ϊʱ��72M��72����1��s
  SysTick->CTRL=0x00000005; 				                 //ʱ����Դ��ΪΪHCLK(72M)���򿪶�ʱ��
  while(1) 
		{                 
			while(!(SysTick->CTRL&0x00010000));              //�ȴ�������0
			SysTick->CTRL &= ~0x00010000;
			cnt--;
			if(cnt == 0) break;
		}
  SysTick->CTRL=0x00000004;                         //�رն�ʱ��
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

