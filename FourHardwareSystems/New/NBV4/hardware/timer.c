#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "timer.h"

//定时3ms，modbus通信协议的时间
void Timer1Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_DeInit(TIM1);                    					                    // 复位TIMx定时器 
	TIM_TimeBaseStructure.TIM_Period = 40;                            // 每T秒更新一次子程序 T5<30
	TIM_TimeBaseStructure.TIM_Prescaler =7199;                        // 分频36000       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;           // 时钟分频  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;       // 计数方向向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                  // 重复计数  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                         //使能TIMx中断源 
	TIM_Cmd(TIM1, ENABLE);                                             //使能定时器1
}


//定时1s，定时处理TouchScreen的数据
void Tim2Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 2000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =(36000-1); //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ClearFlag(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    //使能或者失能指定的TIM中断//TIM2 //使能
	TIM_Cmd(TIM2, ENABLE);             //使能定时器中断
}
