#include "digital_io.h"

 void GpioRcc_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOE,ENABLE);	
}

void Gpio_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
}
 
 void IO_Configuration(void)
 {
	 GpioRcc_Configuration();
	 Gpio_Configuration();
 }

 void DPortOut(u8 port,u8 value)
{
	u16 channel;
  switch(port)
		{
			case 0: channel=GPIO_Pin_8;break;
      case 1: channel=GPIO_Pin_9;break;
      case 2: channel=GPIO_Pin_10;break;
      case 3: channel=GPIO_Pin_11;break;
      case 4: channel=GPIO_Pin_12;break;
      case 5: channel=GPIO_Pin_13;break;
      case 6: channel=GPIO_Pin_14;break;
      case 7: channel=GPIO_Pin_15;break;
      default: break;
    }
	if(value==0)
		GPIO_ResetBits(GPIOE,channel);
   else
		GPIO_SetBits(GPIOE,channel);              
} 

u8 DPortIn(u8 port)
{
	u8 value;
  u16 tep;    	 
  switch(port)
		{
			case 0: tep=GPIO_Pin_8;break;
      case 1: tep=GPIO_Pin_9;break;
      case 2: tep=GPIO_Pin_10;break;
      case 3: tep=GPIO_Pin_11;break;
      case 4: tep=GPIO_Pin_12;break;
      case 5: tep=GPIO_Pin_13;break;
      case 6: tep=GPIO_Pin_14;break;
      case 7: tep=GPIO_Pin_15;break;
      default: break;
    }
  value=GPIO_ReadInputDataBit(GPIOF,tep);   
  return value;
}

void DPortCOut(u8 arr[8])
{
	u8 i;
  for(i=0;i<8;i++)
  {
		DPortOut(i,arr[i]); 
  }     
} 

void DPortCIn(u8 arr[8])
{
	u8 i;
  for(i=0;i<8;i++)
  {
		arr[i]=DPortIn(i);
  }
} 
