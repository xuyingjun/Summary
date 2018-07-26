#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "timer.h"

//��ʱ3ms��modbusͨ��Э���ʱ��
void Timer1Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_DeInit(TIM1);                    					                    // ��λTIMx��ʱ�� 
	TIM_TimeBaseStructure.TIM_Period = 40;                            // ÿT�����һ���ӳ��� T5<30
	TIM_TimeBaseStructure.TIM_Prescaler =7199;                        // ��Ƶ36000       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;           // ʱ�ӷ�Ƶ  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;       // �����������ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                  // �ظ�����  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                         //ʹ��TIMx�ж�Դ 
	TIM_Cmd(TIM1, ENABLE);                                             //ʹ�ܶ�ʱ��1
}


//��ʱ1s����ʱ����TouchScreen������
void Tim2Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 2000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =(36000-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ClearFlag(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    //ʹ�ܻ���ʧ��ָ����TIM�ж�//TIM2 //ʹ��
	TIM_Cmd(TIM2, ENABLE);             //ʹ�ܶ�ʱ���ж�
}
