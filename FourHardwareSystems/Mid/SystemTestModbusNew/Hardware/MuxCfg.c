#include "MuxCfg.h"
void AiMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PR端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟

	GPIO_InitStructure.GPIO_Pin = Ai1Mux_Pin|Ai2Mux_Pin|Ai3Mux_Pin|Ai4Mux_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,Ai1Mux_Pin|Ai2Mux_Pin|Ai3Mux_Pin|Ai4Mux_Pin);
}
void AoMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = Ao1Mux_Pin|Ao2Mux_Pin|Ao3Mux_Pin|Ao4Mux_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,Ao1Mux_Pin|Ao2Mux_Pin|Ao3Mux_Pin|Ao4Mux_Pin);
}
void AiChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent)
{
	switch(MuxVoltageOrCurrent)
	{
		
		case MUX_CURRENT:
			     switch(Channel)
					 {
						 case 1:GPIO_ResetBits(Ai1Mux_Port,Ai1Mux_Pin);break;
						 case 2:GPIO_ResetBits(Ai2Mux_Port,Ai2Mux_Pin);break;
						 case 3:GPIO_ResetBits(Ai3Mux_Port,Ai3Mux_Pin);break;
						 case 4:GPIO_ResetBits(Ai4Mux_Port,Ai4Mux_Pin);break;
						 default:break;
					 }
					 break;
		case MUX_VOLTAGE:
			     switch(Channel)
					 {
						 case 1:GPIO_SetBits(Ai1Mux_Port,Ai1Mux_Pin);break;
						 case 2:GPIO_SetBits(Ai2Mux_Port,Ai2Mux_Pin);break;
						 case 3:GPIO_SetBits(Ai3Mux_Port,Ai3Mux_Pin);break;
						 case 4:GPIO_SetBits(Ai4Mux_Port,Ai4Mux_Pin);break;
						 default:break;
					 }
					 break;
		default:break;
	}
}
void AoChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent)
{
	switch(MuxVoltageOrCurrent)
	{
		case MUX_CURRENT:
			     switch(Channel)
					 {
						 case 1:GPIO_ResetBits(Ao1Mux_Port,Ao1Mux_Pin);break;
						 case 2:GPIO_ResetBits(Ao2Mux_Port,Ao2Mux_Pin);break;
						 case 3:GPIO_ResetBits(Ao3Mux_Port,Ao3Mux_Pin);break;
						 case 4:GPIO_ResetBits(Ao4Mux_Port,Ao4Mux_Pin);break;
						 default:break;
					 }
					 break;
		case MUX_VOLTAGE:
			     switch(Channel)
					 {
						 case 1:GPIO_SetBits(Ao1Mux_Port,Ao1Mux_Pin);break;
						 case 2:GPIO_SetBits(Ao2Mux_Port,Ao2Mux_Pin);break;
						 case 3:GPIO_SetBits(Ao3Mux_Port,Ao3Mux_Pin);break;
						 case 4:GPIO_SetBits(Ao4Mux_Port,Ao4Mux_Pin);break;
						 default:break;
					 }
					 break;
		default:break;
	}
}
void MuxGpioCfg(void)
{
	AiMuxGpioCfg();
	AoMuxGpioCfg();
}

