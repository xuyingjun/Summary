#include "stm32f10x.h"
#include "Globle.h"
#include "stm32f10x_usart.h"
#include "usart.h"

void UART_Configuration()
{
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(USART1_GPIO_CLK  , ENABLE);
	RCC_APB2PeriphClockCmd(USART2_GPIO_CLK  , ENABLE);
	RCC_APB2PeriphClockCmd(USART3_GPIO_CLK  , ENABLE);
	
	RCC_APB2PeriphClockCmd(USART1_CLK  , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO  , ENABLE);
	
	RCC_APB1PeriphClockCmd(USART2_CLK  , ENABLE);
	RCC_APB1PeriphClockCmd(USART3_CLK  , ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);  
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  
  /*����1 RX�ܽ�����*/
  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USART1_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

  /*����2 RX�ܽ�����*/
  /* Configure USART2 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USART2_RxPin;
  GPIO_Init(USART2_GPIO, &GPIO_InitStructure);  

  /*����3 RX�ܽ�����*/
  /* Configure USART3 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USART3_RxPin;
  GPIO_Init(USART3_GPIO, &GPIO_InitStructure);



  /*����1 TX�ܽ�����*/ 
  /* Configure USART1 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USART1_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USART2_TxPin;
  GPIO_Init(USART2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = USART3_TxPin;
  GPIO_Init(USART3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  DE1=1;
	DE2=1;
	
/* USART1 and USART2 configuration */

  /* USART1 and USART2 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;               /*���ò�����Ϊ115200*/
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;/*��������λΪ8*/
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     /*����ֹͣλΪ1λ*/
  USART_InitStructure.USART_Parity = USART_Parity_No;        /*����żУ��*/
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;/*��Ӳ������*/
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  /*���ͺͽ���*/

  /*���ô���1 */
  USART_Init(USART1, &USART_InitStructure);

  /*���ô���2*/
	USART_InitStructure.USART_BaudRate = 9600;               /*���ò�����Ϊ115200*/
  USART_Init(USART2, &USART_InitStructure);
  /*���ô���3*/
	USART_InitStructure.USART_BaudRate = 115200;
  USART_Init(USART3, &USART_InitStructure);

  			    
  /*ʹ�ܴ���1�ķ��ͺͽ����ж�*/
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /*ʹ�ܴ���2�ķ��ͺͽ����ж�*/
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

  /* ʹ�ܴ���1 */
  USART_Cmd(USART1, ENABLE);
  /* ʹ�ܴ���2 */
  USART_Cmd(USART2, ENABLE);
  /* ʹ�ܴ���3 */
  USART_Cmd(USART3, ENABLE);

}

void SendStr4851(char *str)
{
	DE1=0;
	delay_ms(1);
	Uart1_SendBuffSize=0;
	while(*str!=0x00)
	{
		Uart1_Send_buffer[Uart1_SendBuffSize++]=*str;
		str++;
	}
	Uart1_SCnt=0;
	USART_ITConfig(USART1,USART_IT_TXE,ENABLE);			  	//
}

void SendStr4852(char *str)
{
	DE2=0;
	delay_ms(1);
	Uart2_SendBuffSize=0;
	while(*str!=0x00)
	{
		Uart2_Send_buffer[Uart2_SendBuffSize++]=*str;
		str++;
	}
	Uart2_SCnt=0;
	USART_ITConfig(USART2,USART_IT_TXE,ENABLE);			  	//
}

void printfStr(char *str)
{		
	Uart3_SendBuffSize=0;
	while(*str!=0x00)
	{
		Uart3_Send_buffer[Uart3_SendBuffSize++]=*str;
		str++;
	}
	Uart3_SCnt=0;
	USART_ITConfig(USART3,USART_IT_TXE,ENABLE);			  	//
			
}

void LedInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
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

void UsendChar(char dat)
{
	DE2=0;
	delay_ms(12);
	USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
	USART_SendData(USART2,dat);
}

void Usart2SendData(int num,char* sendbuffer)
{
	int i;
  for(i=0;i<num;i++)
	{
		UsendChar(sendbuffer[i]);
  }
}
