#include "logic.h"
#include "Globe.h"

u8 NSLH_Alarm()
{
	u8 Alarm_Flag=0;
	
	return Alarm_Flag;
	
}

//system normal mode
u8 NSLH_Start()
{
	u8 alarm=0; 
	//while(ReceiveFlag==0)
	alarm=NSLH_Alarm();
	if(alarm!=0)
		return alarm;
	
	//����Ӧ���豸
	APortOut(CHWV,4.0);   //ˮ����40%
	Delay_ms(1);
//	while(dataTemp[CHWVfb]<=9.5)  //�۲�ˮ������ֵ������ֵ����95%��Ϊˮ��ȫ��
//	{
//		alarm=NSLH_Alarm();
//		if(alarm!=0)
//			return alarm;
//	}
	return 0;
}


//system stop
u8 NSLH_Stop()
{
	u8 alarm=0;
	alarm=NSLH_Alarm();
	if(alarm!=0)
		return alarm;

	APortOut(CHWV,0.0);    //��ˮ��
	APortOut(ERVFan,0.0);    //��ERV
	APortOut(PTBFan1,0.0);   //��PTB Fan
	APortOut(PTBFan2,0.0);

	return 0;	
}

void NSLH_Clear(void)
{
	int j;
	for(j=0;j<8;j++)
	{
		APortOut(j,0);
		Delay_ms(1);
		
		DPortOut(j,0);
		Delay_ms(1);		
	}
}


//co2 control
void CO2Control(void)
{
		if(CO2<800)                        //Ũ�ȵ���600ppm���ر�ERV����
		{
			DPortOut(ERVFan,0);
			ERVState=0;
		}
		else if(CO2>=800)                   //Ũ�ȸ���800ppm����ERV���ȣ�����Ϊ30%
		{
			DPortOut(ERVFan,1);		//ERV��30%
			ERVState=1;
		}
}

void RoomTempControl(float value)
{
	if(value>25.0)
		{
			APortOut(PTBFan1,8.0);    //PTB Fan ��80%
			APortOut(PTBFan2,8.0);
		}
	else if(value<24.5)
	{
			APortOut(PTBFan1,0.0);    //PTB Fan �ر�
			APortOut(PTBFan2,0.0);
	}
}

//occupined: Occupined sensor 0---UnOccupined Mode  1---Occupined
//occupined=DPortIn(OccuState)
void Get_value(float chwOut,float diffPre,u8 BMS)
{
	chwOutTemp=chwOut;
	DiffPressure=diffPre;
	BMSControl=BMS;
	ReceiveFlag=1;                          //Data Receive Flag: 1---Received  0---Not Received
}

//����ģʽ
u8 OccupanyMode(void)
{
	u8 alarm=0;
	alarm=NSLH_Alarm();
	if(alarm!=0)
		return alarm;
	
	APortOut(CHWV,ValvePosition);		//PID control valve
	
	if(RoomTemp>=RoomTempSv+5)          //�������ݴ�С��������10
	{
		APortOut(PTBFan1,8.0);
		APortOut(PTBFan2,8.0);
	}
	else
	{
		APortOut(PTBFan1,0.0);
		APortOut(PTBFan2,0.0);
	}
	
	return 0;
}

//����ģʽ
u8 Unoccupanied(void)
{
	u8 alarm=0;
	alarm=NSLH_Alarm();
	if(alarm!=0)
		return alarm;
	
	APortOut(CHWV,ValvePosition);		//PID control valve
	return 0;
}
