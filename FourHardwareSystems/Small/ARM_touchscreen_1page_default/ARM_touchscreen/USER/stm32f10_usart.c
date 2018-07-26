#include "stm32f10_usart.h"

//rcc-rcc-rcc-rcc-rcc-rcc-rcc-rcc-rcc-rcc-rcc//
void UsartRcc_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

}
//nvic-nvic-nvic-nvic-nvic-nvic-nvic-nvic-nvic-nvic//
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;//中断结构体
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//设置中断向量表  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //分组

	 	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);


//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//设置串口1中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
//    NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

   	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

	 NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7; // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//
	NVIC_Init(&NVIC_InitStructure);

}
void UsartGPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 



}
void Usart1_Rx(void)
{
//	delay_ms(1);  
//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
void Usart1_Tx(void)
{	 
//   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
//   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

void Usart2_Rx(void)
{ 
   GPIO_InitTypeDef GPIO_InitStruct;
    delay_ms(1);	 
    GPIO_SetBits(GPIOA,GPIO_Pin_12); 
//	USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct)  ;

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

}
void Usart2_Tx(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   	delay_ms(1); 
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	//USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
	

   
}


void Usart_Configuration(void)
{
	USART_InitTypeDef USART_InitStruct;

	UsartRcc_Configuration();
	UsartGPIO_Configuration();
	NVIC_Configuration();
	USART_DeInit(USART1);
	USART_DeInit(USART2);

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct); 		 
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_Cmd(USART1, ENABLE);	
		

	USART_Init(USART2, &USART_InitStruct);	
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_Cmd(USART2, ENABLE);
}
void Usart1ST(void)
{
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
//	USART_ITConfig(USART1,USART_IT_TXE,ENABLE);			  	//

   // Usart_Rx();   
}
void Usart2ST(void)
{
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	Usart2_Rx(); 
} 
void UsartSP(void)
{
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	Usart1_Rx();     
} 
//void UartSP(void)
//{
//    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//    Uart_Rx();
//}
