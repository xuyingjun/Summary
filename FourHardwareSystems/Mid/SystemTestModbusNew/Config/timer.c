#include "timer.h"

/*********************************************************************************************************
** Functoin name:       Time_Configuration
** Descriptions:        ��ʼ��TIM1
                        ���� ʱ�ӡ�TIM1������ʽ����
** input paraments:     ��
** output paraments:    ��    
** Returned values:     ��
*********************************************************************************************************/
void InitializeTimer(int n,float time)
{   
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TypeDef* TIMx;
	
	//++n ���Լ�1����ʹ��n�ۼӺ����ֵ
	switch(++n)
		{
			case 1: TIMx = TIM1; RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); break;
			case 2: TIMx = TIM2;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	break;
			case 3: TIMx = TIM3;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	break;
			case 4: TIMx = TIM4;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	break;        
			case 5: TIMx = TIM5;	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	break;
			default: break;
		}
		
  TIM_DeInit(TIMx);                    					                    // ��λTIMx��ʱ�� 
	TIM_TimeBaseStructure.TIM_Period = (int)(2000 * time);            // ÿT�����һ���ӳ��� T5<30
	TIM_TimeBaseStructure.TIM_Prescaler = (36000-1);                  // ��Ƶ36000       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;           // ʱ�ӷ�Ƶ  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;       // �����������ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                  // �ظ�����  
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIMx, TIM_FLAG_Update); 
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);                         //ʹ��TIMx�ж�Դ 
	TIM_Cmd(TIMx, DISABLE); 
}

/*********************************************************************************************************
** Functoin name:       TimeOn/Off Timx
** Descriptions:        �򿪻�ر�TIMx                        
** input paraments:     ��
** output paraments:    ��    
** Returned values:     ��
*********************************************************************************************************/
void TurnOnTim(int x)
{
   int i = x + 1;
   switch(i)
		 {
			 case 1:	TIM_Cmd(TIM1, ENABLE);break;     //TIM1 ����
			 case 2:	TIM_Cmd(TIM2, ENABLE);break;     //TIM2 ����
			 case 3:	TIM_Cmd(TIM3, ENABLE);break;     //TIM3 ����
			 case 4:	TIM_Cmd(TIM4, ENABLE);break;     //TIM4 ����
			 case 5:	TIM_Cmd(TIM5, ENABLE);break;     //TIM5 ����
			 default: break;
		 } 
}

void TurnOffTim(int x)
{
  int i = x + 1;
  switch(i)
		{
			case 1:	TIM_Cmd(TIM1, DISABLE);break;     //TIM1 �ر�
			case 2:	TIM_Cmd(TIM2, DISABLE);break;     //TIM2 �ر�
			case 3:	TIM_Cmd(TIM3, DISABLE);break;     //TIM3 �ر�
			case 4:	TIM_Cmd(TIM4, DISABLE);break;     //TIM4 �ر�
			case 5:	TIM_Cmd(TIM5, DISABLE);break;     //TIM5 �ر�
			default: break;
		} 
}

void Tim2Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 40; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ClearFlag(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    //ʹ�ܻ���ʧ��ָ����TIM�ж�//TIM2 //ʹ��
	TIM_Cmd(TIM2, ENABLE);             //ʹ�ܶ�ʱ���ж�
}
