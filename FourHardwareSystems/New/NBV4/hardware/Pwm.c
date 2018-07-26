#include "Pwm.h"
#include "stm32f10x_tim.h"

u16 arrSave=0;

void PWM1_Init(u16 arr)  //arr Ϊ���� 1LSB=100us  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  arrSave=arr;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	                        //ʹ�ܶ�ʱ��3ʱ��
																																								//ʹ��GPIO�����AFIO���ù���ģ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);                             //Timer3������ӳ��  TIM3_CH2->B5    
 
   /*���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	��ȫ��ӳ��GPIOC.6*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                     //TIM3_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                               //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);                                        //��ʼ��GPIOC
	 
   /*��ʼ��TIM3*/
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim,TIM3ʱ��Ϊ72MHZ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/*��ʼ��TIM3 Channel1 PWMģʽ*/	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

void PWM1_SetHighTime(u16 HighTime)     //PWM1 ��������ߵ�ƽʱ��
{
	if(HighTime>arrSave)
		HighTime=arrSave;
	TIM_SetCompare1(TIM3,HighTime);
}


void PWM2_Init(u16 arr)  //arr Ϊ���� 1LSB=100us  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  arrSave=arr;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	                        //ʹ�ܶ�ʱ��4ʱ��
																																								//ʹ��GPIO�����AFIO���ù���ģ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);                                //Timer4������ӳ��  TIM3_CH1->D12    
 
   /*���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��	GPIOD.12*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                   //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                              //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);                                       //��ʼ��GPIOD
	 
   /*��ʼ��TIM3*/
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim,TIM3ʱ��Ϊ72MHZ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/*��ʼ��TIM3 Channel2 PWMģʽ*/	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
}

void PWM2_SetHighTime(u16 HighTime)     //PWM2 ��������ߵ�ƽʱ��
{
	if(HighTime>arrSave)
		HighTime=arrSave;
	TIM_SetCompare1(TIM4,HighTime);
}
