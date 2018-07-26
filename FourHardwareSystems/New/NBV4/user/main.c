#include "config.h"
#include "modbusNew.h"

int i;
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

int main(void)
{
	RCC_Configuration();
	MuxGpioCfg();
	stm32ads1115_1_init();
	DAC8554_GPIO_Config();
	DioCfg();
	RTD_init();
	LedInit();
	UART_Configuration();
	Timer1Config();
	Tim2Configure();
	NVIC_Configuration();
	Modbus_RegMap();
	
	while(1)
	{
		RS485_Service();
		APortIn(2);
		APortOut(1,8.0);
		APortOut(2,8.0);
		DPortOut(1,1);
		DigitalIn=DPortIn();
		
		PWM1_Init(100);                     //����PWM1�������100*100us           
		PWM1_SetHighTime(70);               //����PWM1����ߵ�ƽʱ��Ϊ70*100us 
	
		LED=0;		
	}
	
}


void RCC_Configuration()
{
	ErrorStatus HSEStartUpStatus;                              //�����ⲿ���پ�������״̬ö�ٱ�����
	RCC_DeInit();                                              //��λRCC�Ĵ�����Ĭ��ֵ��
	RCC_HSEConfig(RCC_HSE_ON);                                 //���ⲿ�ĸ��پ��� 
	HSEStartUpStatus = RCC_WaitForHSEStartUp();                //�ȴ��ⲿ����ʱ��׼����
	if(HSEStartUpStatus == SUCCESS)
		{
			RCC_HCLKConfig(RCC_SYSCLK_Div1);                         //����AHB
			RCC_PCLK2Config(RCC_HCLK_Div1);                          //����APB2 
			RCC_PCLK1Config(RCC_HCLK_Div2);                          //����APB1 
			//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	      
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //PLL=8M*9=72M
			RCC_PLLCmd(ENABLE);                                      //ʱ��PLL
			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);      //�ȴ�PLL����	      
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);               //����ϵͳʱ��=PLL	  
			while(RCC_GetSYSCLKSource() != 0x08);                    //���PLLʱ���Ƿ���Ϊϵͳʱ��	       
		}
}

void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�����ж�������λ�ú�ƫ����  
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);    //������λ��FLASH�����������ַ��ƫ����Ϊ0 
  
	//�������ȼ����飺��ռ���ȼ��ʹ����ȼ���
	//������������ȼ�����λ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //��ռ���ȼ�4λ�������ȼ�0λ

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;          //TIM1�ж�   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�3	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;            //TIM2�ж�   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�4 //TIM1�����ȼ���TIM2�����ȼ�Ҫ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);
}

