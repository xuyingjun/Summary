#include "Dio.h"

void DiGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PE端口时钟

	GPIO_InitStructure.GPIO_Pin = Di0_Pin|Di1_Pin|Di2_Pin|Di3_Pin|Di4_Pin|Di5_Pin|Di6_Pin|Di7_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(Di_Port, &GPIO_InitStructure);
}

void DoMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PA端口时钟
	
	GPIO_ResetBits(Do_Port,Do1_Pin|Do2_Pin|Do3_Pin|Do4_Pin|Do5_Pin|Do6_Pin|Do7_Pin|Do8_Pin);
	
	GPIO_InitStructure.GPIO_Pin = Do1_Pin|Do2_Pin|Do3_Pin|Do4_Pin|Do5_Pin|Do6_Pin|Do7_Pin|Do8_Pin;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(Do_Port, &GPIO_InitStructure);
}

u8 GetDiMap(void)
{
	u16 temp; 
	temp=GPIO_ReadInputData(Di_Port);
	return (temp>>8)&0xff;
}
void SetSpdtState(u8 channel,u8 State)
{
	switch(State)
	{
		case SpdtOFF:
								switch(channel)
								{
									case 1:GPIO_ResetBits(Do_Port,Do1_Pin);break;
									case 2:GPIO_ResetBits(Do_Port,Do2_Pin);break;
									case 3:GPIO_ResetBits(Do_Port,Do3_Pin);break;
									case 4:GPIO_ResetBits(Do_Port,Do4_Pin);break;
									case 5:GPIO_ResetBits(Do_Port,Do5_Pin);break;
									case 6:GPIO_ResetBits(Do_Port,Do6_Pin);break;
									case 7:GPIO_ResetBits(Do_Port,Do7_Pin);break;
									case 8:GPIO_ResetBits(Do_Port,Do8_Pin);break;
									default:break;									
								}
								break;
		case SpdtON:
								switch(channel)
								{
									case 1:GPIO_SetBits(Do_Port,Do1_Pin);break;
									case 2:GPIO_SetBits(Do_Port,Do2_Pin);break;
									case 3:GPIO_SetBits(Do_Port,Do3_Pin);break;
									case 4:GPIO_SetBits(Do_Port,Do4_Pin);break;
									case 5:GPIO_SetBits(Do_Port,Do5_Pin);break;
									case 6:GPIO_SetBits(Do_Port,Do6_Pin);break;
									case 7:GPIO_SetBits(Do_Port,Do7_Pin);break;
									case 8:GPIO_SetBits(Do_Port,Do8_Pin);break;
									default:break;									
								}
								break;
		default:break;
	}
}

void DioCfg(void)
{
	DoMuxGpioCfg();
	DiGpioCfg();
}


