#include "timer.h"

/*********************************************************************************************************
** Functoin name:       Time_Configuration
** Descriptions:        初始化TIM1
                        包括 时钟、TIM1工作方式配置
** input paraments:     无
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void InitializeTimer(int n,float time)
{   
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TypeDef* TIMx;
	
	//++n 先自加1，再使用n累加后的数值
	switch(++n)
		{
			case 1: TIMx = TIM1; RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); break;
			case 2: TIMx = TIM2;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	break;
			case 3: TIMx = TIM3;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	break;
			case 4: TIMx = TIM4;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	break;        
			case 5: TIMx = TIM5;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	break;
			default: break;
		}
		
  TIM_DeInit(TIMx);                    					                    // 复位TIMx定时器 
	TIM_TimeBaseStructure.TIM_Period = (int)(2000 * time);            // 每T秒更新一次子程序 T5<30
	TIM_TimeBaseStructure.TIM_Prescaler = (36000-1);                  // 分频36000       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;           // 时钟分频  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;       // 计数方向向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                  // 重复计数  
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIMx, TIM_FLAG_Update); 
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);                         //使能TIMx中断源 
	TIM_Cmd(TIMx, DISABLE); 
}

/*********************************************************************************************************
** Functoin name:       TimeOn/Off Timx
** Descriptions:        打开或关闭TIMx                        
** input paraments:     无
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void TurnOnTim(int x)
{
   int i = x + 1;
   switch(i)
		 {
			 case 1:	TIM_Cmd(TIM1, ENABLE);break;     //TIM1 开启
			 case 2:	TIM_Cmd(TIM2, ENABLE);break;     //TIM2 开启
			 case 3:	TIM_Cmd(TIM3, ENABLE);break;     //TIM3 开启
			 case 4:	TIM_Cmd(TIM4, ENABLE);break;     //TIM4 开启
			 case 5:	TIM_Cmd(TIM5, ENABLE);break;     //TIM5 开启
			 default: break;
		 } 
}

void TurnOffTim(int x)
{
  int i = x + 1;
  switch(i)
		{
			case 1:	TIM_Cmd(TIM1, DISABLE);break;     //TIM1 关闭
			case 2:	TIM_Cmd(TIM2, DISABLE);break;     //TIM2 关闭
			case 3:	TIM_Cmd(TIM3, DISABLE);break;     //TIM3 关闭
			case 4:	TIM_Cmd(TIM4, DISABLE);break;     //TIM4 关闭
			case 5:	TIM_Cmd(TIM5, DISABLE);break;     //TIM5 关闭
			default: break;
		} 
}

void Tim2Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 40; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ClearFlag(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    //使能或者失能指定的TIM中断//TIM2 //使能
	TIM_Cmd(TIM2, ENABLE);             //使能定时器中断
}
