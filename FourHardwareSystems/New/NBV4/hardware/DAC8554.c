#include "stm32f10x.h"
#include "DAC8554.h"
u16 Dat;
void DAC8554_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA端口时钟

	GPIO_InitStructure.GPIO_Pin = SYNC_Pin | SCLK_Pin | DIN_Pin | LDAC_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,LDAC_Pin);
}

void DAC8554_delay(void)
{
	int x = 10;
  while(--x);
}

void DAC8554_Send_24_bits(unsigned int data)  // data是一个24位的数据,STM32中int型是32位的数据
{
	char i=0;

	////////////////////////////////
	SYNC_1();
	DAC8554_delay();//////////SYNC脚一个高脉冲
	SYNC_0();
	///////////////////////
	for( i=0; i<24; i++ )
	{
		if((data&0x800000)  == 0x800000)
			DIN_1();  		
		else
			DIN_0();
			
		SCLK_1();
		DAC8554_delay();
		SCLK_0();
		data=data<<1;
	}	
}
/*****************************************************
Parm:需要输出的电压或者电流值 浮点型 电压0.0-10.0  电流4.0-20.0
channel：指示输出AO通道 1~8
CurrentFlag：指示电流输出还是电压输出 0：电压输出 1：电流输出
******************************************************/
void SetAoOut(float Parm,u8 channel,u8 CurrentFlag)
{
	if(Parm<0)Parm=0;
	switch(CurrentFlag)
	{
		case 0:if(Parm>9.999)Parm=9.999;
						break;
		case 1:if(Parm>19.999)Parm=19.999;
						break;
		default:break;
	}
	switch(CurrentFlag)
	{
		case 0:Dat=Parm/2.0/5.0*65536;break;
		case 1:
			     if(Parm<=3.98406)                              //3.98406=20*10/(10+40.2)
					 {
						 Dat=0;
					 }
					 else
					 {
						 Dat=(Parm-3.98406)/(20.08-3.98406)*10/2/5*65536;break;
					 }
		default:Dat=0;
	}
	switch(channel)
	{
		case 1:DAC8554_Send_24_bits((0x00100000|0x00000000)+Dat);break;
		case 2:DAC8554_Send_24_bits((0x00100000|0x00020000)+Dat);break;
		case 3:DAC8554_Send_24_bits((0x00100000|0x00040000)+Dat);break;
		case 4:DAC8554_Send_24_bits((0x00100000|0x00060000)+Dat);break;
		case 5:DAC8554_Send_24_bits((0x00500000|0x00000000)+Dat);break;
		case 6:DAC8554_Send_24_bits((0x00500000|0x00020000)+Dat);break;
		case 7:DAC8554_Send_24_bits((0x00500000|0x00040000)+Dat);break;
		case 8:DAC8554_Send_24_bits((0x00500000|0x00060000)+Dat);break;
		default:break;
	}
}	
