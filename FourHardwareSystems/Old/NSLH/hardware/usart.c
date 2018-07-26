#include "usart.h"
#include "Globe.h"

/*********************************************************************************************************
* Functoin name:       Usart_SendChar
* Descriptions:        USART发送一个字符
* input paraments:     USARTx：用到的串口号 
*						dat：需要发送的数据
* output paraments:    无    
* Returned values:     无
*	Remark:DE为1处于接收状态，DE为0处于发送状态
*********************************************************************************************************/
void Usart_SendChar(char dat)
{  
	DE1=0;     //发送状态
	delay_usart();
	USART_SendData(USART1,dat);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	DE1=1;    //接收状态
}

void Usart2_SendChar(char dat)
{  
	DE2=0;      //发送状态
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
* Descriptions:        USART发送num个字符
* input paraments:     num：发送数据的个数
*					   sendbuffer：待发送数据的缓存数组
* output paraments:    无    
* Returned values:     无
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

//串口发送延时函数
void delay_usart(void)
{
	u8 i=25;
  while(i--);
}

/*********************************************************************************************************
* Functoin name:       Usart_GetCahr
* Descriptions:        USART接收一个字符
* input paraments:     USARTx：用到的串口号
* output paraments:    无    
* Returned values:     dat:接收到的数据
*********************************************************************************************************/
char Usart_GetChar(USART_TypeDef* USARTx)
{
	char dat;
	//while(!USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
	dat = (char)(USART_ReceiveData(USARTx)&0xff);	
	return dat;		
}

//中断向量表，串口和定时器初始化配置
/*********************************************************************************************************
* Functoin name:       Usart_Total_Initi
* Descriptions:        完成USART通信的所有初始化
*                      包括：NVIC中断嵌套 TIM1定时与中断 USART1初始化						  
* input paraments:     无
* output paraments:    无    
* Returned values:     无
*********************************************************************************************************/
void Usart_Total_Initi(void)
{
  NVIC_Configuration();
	Usart_GPIO_Configuration();
}

/*********************************************************************************************************
* Functoin name:       Usart_GPIO_Configuration
* Descriptions:        初始化USart
*					   包括：时钟、IO、USART工作方式配置
* input paraments:     无  
* output paraments:    无    
* Returned values:     无
*********************************************************************************************************/				   
void Usart_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	// 打开GPIOA与USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//打开GPIOD和USART2的时钟，同时需要映射管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        
	//串口USART1的IO初始化	GPIOA 9--TX 10--RX 8--DE
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
	
	//USART2管家配置 PD5--TX PD6--RX PD7--DE
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

	DE1=1;      //接收状态
	DE2=1;      //接收状态

	//串口USART1配置初始化
	USART_DeInit(USART1);
	USART_DeInit(USART2);
	
	//串口波特率等信息设置，USART1和USART2配置相同
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
  //使能USART1接收中断和使能USART1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1,USART_IT_PE,ENABLE);
	USART_ITConfig(USART1,USART_IT_ERR,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	USART_ClearFlag(USART2,USART_FLAG_TC);   
  //使能USART1接收中断和使能USART1
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
	
	//设置中断向量表位置和偏移量  
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);    //向量表位于FLASH，向量表基地址的偏移量为0 
	
	//设置优先级分组：先占优先级和从优先级。
	//输入参数：优先级分组位长度
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //先占优先级4位，从优先级0位
  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;          //USART1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //UART2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;          //TIM1中断   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级3	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;            //TIM2中断   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级4 //TIM1的优先级比TIM2的优先级要高
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;             //TIM3中断   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  //抢占优先级5 //TIM3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;            //TIM4中断   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; //抢占优先级6 //TIM4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;            //TIM5中断   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; //抢占优先级7 //TIM5
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);
}					 

void  TurnOnUsart1(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	delay_usart();
	DE1=1;  //使USART1常处于接收状态
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
	//GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //使USART2常处于接收状态
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
	DE2=0;  //发送状态
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
