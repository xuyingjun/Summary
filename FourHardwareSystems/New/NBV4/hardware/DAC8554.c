#include "stm32f10x.h"
#include "DAC8554.h"
u16 Dat;
void DAC8554_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = SYNC_Pin | SCLK_Pin | DIN_Pin | LDAC_Pin;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,LDAC_Pin);
}

void DAC8554_delay(void)
{
	int x = 10;
  while(--x);
}

void DAC8554_Send_24_bits(unsigned int data)  // data��һ��24λ������,STM32��int����32λ������
{
	char i=0;

	////////////////////////////////
	SYNC_1();
	DAC8554_delay();//////////SYNC��һ��������
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
Parm:��Ҫ����ĵ�ѹ���ߵ���ֵ ������ ��ѹ0.0-10.0  ����4.0-20.0
channel��ָʾ���AOͨ�� 1~8
CurrentFlag��ָʾ����������ǵ�ѹ��� 0����ѹ��� 1���������
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
