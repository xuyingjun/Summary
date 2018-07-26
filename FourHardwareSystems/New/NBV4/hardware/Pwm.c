#include "Pwm.h"
#include "stm32f10x_tim.h"

u16 arrSave=0;

void PWM1_Init(u16 arr)  //arr 为周期 1LSB=100us  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  arrSave=arr;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	                        //使能定时器3时钟
																																								//使能GPIO外设和AFIO复用功能模块时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);                             //Timer3部分重映射  TIM3_CH2->B5    
 
   /*设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	完全重映像GPIOC.6*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                     //TIM3_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                               //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);                                        //初始化GPIOC
	 
   /*初始化TIM3*/
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim,TIM3时钟为72MHZ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/*初始化TIM3 Channel1 PWM模式*/	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

void PWM1_SetHighTime(u16 HighTime)     //PWM1 设置输出高电平时间
{
	if(HighTime>arrSave)
		HighTime=arrSave;
	TIM_SetCompare1(TIM3,HighTime);
}


void PWM2_Init(u16 arr)  //arr 为周期 1LSB=100us  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  arrSave=arr;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	                        //使能定时器4时钟
																																								//使能GPIO外设和AFIO复用功能模块时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);                                //Timer4部分重映射  TIM3_CH1->D12    
 
   /*设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOD.12*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                   //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                              //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);                                       //初始化GPIOD
	 
   /*初始化TIM3*/
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim,TIM3时钟为72MHZ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/*初始化TIM3 Channel2 PWM模式*/	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
}

void PWM2_SetHighTime(u16 HighTime)     //PWM2 设置输出高电平时间
{
	if(HighTime>arrSave)
		HighTime=arrSave;
	TIM_SetCompare1(TIM4,HighTime);
}
