#include "MuxCfg.h"

//AI端口
void AiMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PR端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟
	
	//AI1-12  ----- PF0-PF7 PE2-PE5
	GPIO_InitStructure.GPIO_Pin = Ai1Mux_Pin|Ai2Mux_Pin|Ai3Mux_Pin|Ai4Mux_Pin|Ai5Mux_Pin|Ai6Mux_Pin|Ai7Mux_Pin|Ai8Mux_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//设置指定的数据端口位，  置1
	GPIO_SetBits(GPIOF,Ai1Mux_Pin|Ai2Mux_Pin|Ai3Mux_Pin|Ai4Mux_Pin|Ai5Mux_Pin|Ai6Mux_Pin|Ai7Mux_Pin|Ai8Mux_Pin);
	
	GPIO_InitStructure.GPIO_Pin = Ai9Mux_Pin|Ai10Mux_Pin|Ai11Mux_Pin|Ai12Mux_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//设置指定的数据端口位， 置1
	GPIO_SetBits(GPIOE,Ai9Mux_Pin|Ai10Mux_Pin|Ai11Mux_Pin|Ai12Mux_Pin);
}

//AO端口
void AoMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//AO1-AO8  -----  PA0-PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = Ao1Mux_Pin|Ao2Mux_Pin|Ao3Mux_Pin|Ao4Mux_Pin|Ao5Mux_Pin|Ao6Mux_Pin|Ao7Mux_Pin|Ao8Mux_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//设置指定的数据端口位，置1
	GPIO_SetBits(GPIOA,Ao1Mux_Pin|Ao2Mux_Pin|Ao3Mux_Pin|Ao4Mux_Pin|Ao5Mux_Pin|Ao6Mux_Pin|Ao7Mux_Pin|Ao8Mux_Pin);
}

//AI通道选择
void AiChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent)
{
	//电流信号
	switch(MuxVoltageOrCurrent)
	{
		//如果输入是电流信号，端口数据置1，默认是输入电流信号
		case MUX_CURRENT:
			     switch(Channel)
					 {
						 case 1:GPIO_SetBits(Ai1Mux_Port,Ai1Mux_Pin);break;
						 case 2:GPIO_SetBits(Ai2Mux_Port,Ai2Mux_Pin);break;
						 case 3:GPIO_SetBits(Ai3Mux_Port,Ai3Mux_Pin);break;
						 case 4:GPIO_SetBits(Ai4Mux_Port,Ai4Mux_Pin);break;
						 case 5:GPIO_SetBits(Ai5Mux_Port,Ai5Mux_Pin);break;
						 case 6:GPIO_SetBits(Ai6Mux_Port,Ai6Mux_Pin);break;
						 case 7:GPIO_SetBits(Ai7Mux_Port,Ai7Mux_Pin);break;
						 case 8:GPIO_SetBits(Ai8Mux_Port,Ai8Mux_Pin);break;
						 case 9:GPIO_SetBits(Ai9Mux_Port,Ai9Mux_Pin);break;
						 case 10:GPIO_SetBits(Ai10Mux_Port,Ai10Mux_Pin);break;
						 case 11:GPIO_SetBits(Ai11Mux_Port,Ai11Mux_Pin);break;
						 case 12:GPIO_SetBits(Ai12Mux_Port,Ai12Mux_Pin);break;
						 default:break;
					 }
					 break;
					 
		//如果输入是电压信号，数据端口位置清零		 
		case MUX_VOLTAGE:
			     switch(Channel)
					 {
						 case 1:GPIO_ResetBits(Ai1Mux_Port,Ai1Mux_Pin);break;
						 case 2:GPIO_ResetBits(Ai2Mux_Port,Ai2Mux_Pin);break;
						 case 3:GPIO_ResetBits(Ai3Mux_Port,Ai3Mux_Pin);break;
						 case 4:GPIO_ResetBits(Ai4Mux_Port,Ai4Mux_Pin);break;
						 case 5:GPIO_ResetBits(Ai5Mux_Port,Ai5Mux_Pin);break;
						 case 6:GPIO_ResetBits(Ai6Mux_Port,Ai6Mux_Pin);break;
						 case 7:GPIO_ResetBits(Ai7Mux_Port,Ai7Mux_Pin);break;
						 case 8:GPIO_ResetBits(Ai8Mux_Port,Ai8Mux_Pin);break;
						 case 9:GPIO_ResetBits(Ai9Mux_Port,Ai9Mux_Pin);break;
						 case 10:GPIO_ResetBits(Ai10Mux_Port,Ai10Mux_Pin);break;
						 case 11:GPIO_ResetBits(Ai11Mux_Port,Ai11Mux_Pin);break;
						 case 12:GPIO_ResetBits(Ai12Mux_Port,Ai12Mux_Pin);break;
						 default:break;
					 }
					 break;
					 
		default:break;
	}
}

//AO端口选择
void AoChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent)
{
	switch(MuxVoltageOrCurrent)
	{
		//如果输出是电流信号，端口数据为清零
		case MUX_CURRENT:
			     switch(Channel)
					 {
						 case 1:GPIO_ResetBits(Ao1Mux_Port,Ao1Mux_Pin);break;
						 case 2:GPIO_ResetBits(Ao2Mux_Port,Ao2Mux_Pin);break;
						 case 3:GPIO_ResetBits(Ao3Mux_Port,Ao3Mux_Pin);break;
						 case 4:GPIO_ResetBits(Ao4Mux_Port,Ao4Mux_Pin);break;
						 case 5:GPIO_ResetBits(Ao5Mux_Port,Ao5Mux_Pin);break;
						 case 6:GPIO_ResetBits(Ao6Mux_Port,Ao6Mux_Pin);break;
						 case 7:GPIO_ResetBits(Ao7Mux_Port,Ao7Mux_Pin);break;
						 case 8:GPIO_ResetBits(Ao8Mux_Port,Ao8Mux_Pin);break;
						 default:break;
					 }
					 break;
		
		//如果输出是电压信号，端口数据位置1  默认是输出电压
		case MUX_VOLTAGE:
			     switch(Channel)
					 {
						 case 1:GPIO_SetBits(Ao1Mux_Port,Ao1Mux_Pin);break;
						 case 2:GPIO_SetBits(Ao2Mux_Port,Ao2Mux_Pin);break;
						 case 3:GPIO_SetBits(Ao3Mux_Port,Ao3Mux_Pin);break;
						 case 4:GPIO_SetBits(Ao4Mux_Port,Ao4Mux_Pin);break;
						 case 5:GPIO_SetBits(Ao5Mux_Port,Ao5Mux_Pin);break;
						 case 6:GPIO_SetBits(Ao6Mux_Port,Ao6Mux_Pin);break;
						 case 7:GPIO_SetBits(Ao7Mux_Port,Ao7Mux_Pin);break;
						 case 8:GPIO_SetBits(Ao8Mux_Port,Ao8Mux_Pin);break;
						 default:break;
					 }
					 break;
					 
		default:break;
	}
}

//AI和AO端口初始化配置
void MuxGpioCfg(void)
{
	AiMuxGpioCfg();
	AoMuxGpioCfg();
}

