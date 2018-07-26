#include "logic.h"

//1.ѡ���ѹ������������ 2.��ȡ��������ֵ
void APortIn(u8 channel)
{
	AiChannelSetMux(channel,MUX_VOLTAGE);
	AportIn[channel-1]=ReadAdOfChannel(channel,MODE_VOLTAGE);
}

//1. ѡ���ѹ������� 2. ָ����Ӧ���ֵ
void APortOut(u8 channel, float value)
{
	AoChannelSetMux(channel,MUX_VOLTAGE);
	SetAoOut(value,channel,MODE_VOLTAGE);
}

//��ȡDI�˿���ֵ
u8 DPortIn()
{
	u8 digitalin;
	digitalin=GetDiMap();
	return digitalin;
}

//���ö˿��������
//value----SpdtOFF/SpdtON
void DPortOut(u8 channel, u8 value)
{
	SetSpdtState(channel,value);
}
