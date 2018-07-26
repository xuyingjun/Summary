#include "MuxCfg.h"

//AI�˿�
void AiMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PR�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PE�˿�ʱ��
	
	//AI1-12  ----- PF0-PF7 PE2-PE5
	GPIO_InitStructure.GPIO_Pin = Ai1Mux_Pin|Ai2Mux_Pin|Ai3Mux_Pin|Ai4Mux_Pin|Ai5Mux_Pin|Ai6Mux_Pin|Ai7Mux_Pin|Ai8Mux_Pin;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//����ָ�������ݶ˿�λ��  ��1
	GPIO_SetBits(GPIOF,Ai1Mux_Pin|Ai2Mux_Pin|Ai3Mux_Pin|Ai4Mux_Pin|Ai5Mux_Pin|Ai6Mux_Pin|Ai7Mux_Pin|Ai8Mux_Pin);
	
	GPIO_InitStructure.GPIO_Pin = Ai9Mux_Pin|Ai10Mux_Pin|Ai11Mux_Pin|Ai12Mux_Pin;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//����ָ�������ݶ˿�λ�� ��1
	GPIO_SetBits(GPIOE,Ai9Mux_Pin|Ai10Mux_Pin|Ai11Mux_Pin|Ai12Mux_Pin);
}

//AO�˿�
void AoMuxGpioCfg(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//AO1-AO8  -----  PA0-PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = Ao1Mux_Pin|Ao2Mux_Pin|Ao3Mux_Pin|Ao4Mux_Pin|Ao5Mux_Pin|Ao6Mux_Pin|Ao7Mux_Pin|Ao8Mux_Pin;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����ָ�������ݶ˿�λ����1
	GPIO_SetBits(GPIOA,Ao1Mux_Pin|Ao2Mux_Pin|Ao3Mux_Pin|Ao4Mux_Pin|Ao5Mux_Pin|Ao6Mux_Pin|Ao7Mux_Pin|Ao8Mux_Pin);
}

//AIͨ��ѡ��
void AiChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent)
{
	//�����ź�
	switch(MuxVoltageOrCurrent)
	{
		//��������ǵ����źţ��˿�������1��Ĭ������������ź�
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
					 
		//��������ǵ�ѹ�źţ����ݶ˿�λ������		 
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

//AO�˿�ѡ��
void AoChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent)
{
	switch(MuxVoltageOrCurrent)
	{
		//�������ǵ����źţ��˿�����Ϊ����
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
		
		//�������ǵ�ѹ�źţ��˿�����λ��1  Ĭ���������ѹ
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

//AI��AO�˿ڳ�ʼ������
void MuxGpioCfg(void)
{
	AiMuxGpioCfg();
	AoMuxGpioCfg();
}

