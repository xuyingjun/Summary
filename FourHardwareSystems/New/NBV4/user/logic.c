#include "logic.h"

//1.选择电压电流输入类型 2.读取数据输入值
void APortIn(u8 channel)
{
	AiChannelSetMux(channel,MUX_VOLTAGE);
	AportIn[channel-1]=ReadAdOfChannel(channel,MODE_VOLTAGE);
}

//1. 选择电压电流输出 2. 指定相应输出值
void APortOut(u8 channel, float value)
{
	AoChannelSetMux(channel,MUX_VOLTAGE);
	SetAoOut(value,channel,MODE_VOLTAGE);
}

//读取DI端口数值
u8 DPortIn()
{
	u8 digitalin;
	digitalin=GetDiMap();
	return digitalin;
}

//设置端口数字输出
//value----SpdtOFF/SpdtON
void DPortOut(u8 channel, u8 value)
{
	SetSpdtState(channel,value);
}
