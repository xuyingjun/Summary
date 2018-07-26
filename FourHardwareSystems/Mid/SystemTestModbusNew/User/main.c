#include "config.h"
#include "modbusNew.h"

u8 mode=MODE_VOLTAGE;
unsigned char DiMap;
int i=0;

int main(void)
{	
	RCC_Configuration();
	NVIC_Configuration();
	delay_init();
	ADC1_GPIO_Config();
  ADC1_Mode_Config();
	DioCfg();
	LedInit();
	SPI_Flash_Init();
	DAC7554_GPIO_Config();
	UsartConfigure();
	
	//������ʱ��2����ʱ����Ϊ4ms
	Tim2Configure();
	
	PWM1_Init(100);                     //����PWM1�������100*100us           
	PWM1_SetHighTime(70);               //����PWM1����ߵ�ƽʱ��Ϊ70*100us 
	
	PWM2_Init(100);                     //����PWM2�������100*100us  
	PWM2_SetHighTime(30);               //����PWM2����ߵ�ƽʱ��Ϊ30*100us 
	
	while(1)
	{
		Modbus_RegMap();
		RS485_Service();
		
		LED=0;
		
		SetAoOut(1,1,mode);
		SetAoOut(WriteReg[1],2,mode);
		SetAoOut(WriteReg[2],3,mode);
		SetAoOut(WriteReg[3],4,mode);
		
		//AI Test
		for(i=0;i<=6;i++)
		{
			advalue[i]=ReadAdOfChannel(i,mode);
		}
		temperature = advalue[1]*20;

		//DO Test
		SetSpdtState(1,SpdtOFF);SetSpdtState(3,SpdtOFF);
		SetSpdtState(2,SpdtOFF); SetSpdtState(4,SpdtOFF); 
		
		//DI Test
		DiMap=GetDiMap(); 
		
		for(int k=0;k<6;k++)
		{
			ReadReg[k]=advalue[k+1];
		}
		ReadReg[6]=DiMap&0x01;
		ReadReg[7]=DiMap&0x02;
		ReadReg[8]=DiMap&0x04;
		ReadReg[9]=DiMap&0x08;
		
		LED=1;
	}
}



void RCC_Configuration(void)
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

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�����ж�������λ�ú�ƫ����  
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);    //������λ��FLASH�����������ַ��ƫ����Ϊ0 
	
	//�������ȼ����飺��ռ���ȼ��ʹ����ȼ���
	//������������ȼ�����λ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //��ռ���ȼ�4λ�������ȼ�0λ
  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;          //USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;          	 //TIM2�ж�   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);
}

void LedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE  , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
