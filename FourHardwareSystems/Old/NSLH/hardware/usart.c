#include "usart.h"
#include "Globe.h"

/*********************************************************************************************************
* Functoin name:       Usart_SendChar
* Descriptions:        USART����һ���ַ�
* input paraments:     USARTx���õ��Ĵ��ں� 
*						dat����Ҫ���͵�����
* output paraments:    ��    
* Returned values:     ��
*	Remark:DEΪ1���ڽ���״̬��DEΪ0���ڷ���״̬
*********************************************************************************************************/
void Usart_SendChar(char dat)
{  
	DE1=0;     //����״̬
	delay_usart();
	USART_SendData(USART1,dat);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	DE1=1;    //����״̬
}

void Usart2_SendChar(char dat)
{  
	DE2=0;      //����״̬
	delay_usart();
	USART_SendData(USART2,dat);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  DE2=1;
}

void UsendChar(char dat)
{
	DE2=0;
	Delay_ms(12);
	USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
	USART_SendData(USART2,dat);
}

/*******************D**************************************************************************************
* Functoin name:       void UsartSendData(int num,char* sendbuffer)
* Descriptions:        USART����num���ַ�
* input paraments:     num���������ݵĸ���
*					   sendbuffer�����������ݵĻ�������
* output paraments:    ��    
* Returned values:     ��
*********************************************************************************************************/
void UsartSendData(int num,char* sendbuffer)
{
	int i;
  for(i=0;i<num;i++)
	{
		Usart_SendChar(sendbuffer[i]); 
  }
}

void Usart2SendData(int num,char* sendbuffer)
{
	int i;
  for(i=0;i<num;i++)
	{
		//Usart2_SendChar(sendbuffer[i]); 
		UsendChar(sendbuffer[i]);
  }
}

//���ڷ�����ʱ����
void delay_usart(void)
{
	u8 i=25;
  while(i--);
}

/*********************************************************************************************************
* Functoin name:       Usart_GetCahr
* Descriptions:        USART����һ���ַ�
* input paraments:     USARTx���õ��Ĵ��ں�
* output paraments:    ��    
* Returned values:     dat:���յ�������
*********************************************************************************************************/
char Usart_GetChar(USART_TypeDef* USARTx)
{
	char dat;
	//while(!USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
	dat = (char)(USART_ReceiveData(USARTx)&0xff);	
	return dat;		
}

//�ж����������ںͶ�ʱ����ʼ������
/*********************************************************************************************************
* Functoin name:       Usart_Total_Initi
* Descriptions:        ���USARTͨ�ŵ����г�ʼ��
*                      ������NVIC�ж�Ƕ�� TIM1��ʱ���ж� USART1��ʼ��						  
* input paraments:     ��
* output paraments:    ��    
* Returned values:     ��
*********************************************************************************************************/
void Usart_Total_Initi(void)
{
  NVIC_Configuration();
	Usart_GPIO_Configuration();
}

/*********************************************************************************************************
* Functoin name:       Usart_GPIO_Configuration
* Descriptions:        ��ʼ��USart
*					   ������ʱ�ӡ�IO��USART������ʽ����
* input paraments:     ��  
* output paraments:    ��    
* Returned values:     ��
*********************************************************************************************************/				   
void Usart_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	// ��GPIOA��USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//��GPIOD��USART2��ʱ�ӣ�ͬʱ��Ҫӳ��ܽ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        
	//����USART1��IO��ʼ��	GPIOA 9--TX 10--RX 8--DE
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	//USART2�ܼ����� PD5--TX PD6--RX PD7--DE
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	DE1=1;      //����״̬
	DE2=1;      //����״̬

	//����USART1���ó�ʼ��
	USART_DeInit(USART1);
	USART_DeInit(USART2);
	
	//���ڲ����ʵ���Ϣ���ã�USART1��USART2������ͬ
	USART_InitStruct.USART_BaudRate = 19200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct); 
	
	USART_InitStruct.USART_BaudRate = 9600;
	USART_Init(USART2, &USART_InitStruct); 

  USART_ClearFlag(USART1,USART_FLAG_TC);   
  //ʹ��USART1�����жϺ�ʹ��USART1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1,USART_IT_PE,ENABLE);
	USART_ITConfig(USART1,USART_IT_ERR,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	USART_ClearFlag(USART2,USART_FLAG_TC);   
  //ʹ��USART1�����жϺ�ʹ��USART1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2,USART_IT_PE,ENABLE);
	USART_ITConfig(USART2,USART_IT_ERR,ENABLE);
	USART_Cmd(USART2,ENABLE);	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// <Functoin Descriptions> Complete the configuration of nested interrupts: USART1 USART2 TIM1--TIM5 
///<input paraments>    Null
/// <output paraments>  Null   
/// <Return values>     Null
////////////////////////////////////////////////////////////////////////////////////////////////////  
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�����ж�������λ�ú�ƫ����  
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);    //������λ��FLASH�����������ַ��ƫ����Ϊ0 
	
	//�������ȼ����飺��ռ���ȼ��ʹ����ȼ���
	//������������ȼ�����λ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //��ռ���ȼ�4λ�������ȼ�0λ
  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;          //USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //UART2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
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

	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;             //TIM3�ж�   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  //��ռ���ȼ�5 //TIM3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;            //TIM4�ж�   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; //��ռ���ȼ�6 //TIM4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;            //TIM5�ж�   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; //��ռ���ȼ�7 //TIM5
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);
}					 

void  TurnOnUsart1(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	delay_usart();
	DE1=1;  //ʹUSART1�����ڽ���״̬
}

void  TurnOffUsart1(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	delay_usart();
	//GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

void  TurnOnUsart2(void)
{
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	delay_usart();
	//GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //ʹUSART2�����ڽ���״̬
	DE1=1;
}

void  TurnOffUsart2(void)
{
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	delay_usart();
	//GPIO_SetBits(GPIOD,GPIO_Pin_7);
}

void UsartSendString(u8 arr[])
{
	u8 i;
	DE2=0;  //����״̬
	for (i=0;i<24;i++)
	{
		USART_ClearFlag(USART2,USART_FLAG_TC);
		delay_usart();
		USART_SendData(USART2,arr[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	}
}


/*********************************************************************************************************
*                                        End Of File
*********************************************************************************************************/
