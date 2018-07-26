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
	
	//开相应的设备
	APortOut(CHWV,4.0);   //水阀开40%
	Delay_ms(1);
//	while(dataTemp[CHWVfb]<=9.5)  //观察水阀反馈值，反馈值大于95%认为水阀全开
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

	APortOut(CHWV,0.0);    //关水阀
	APortOut(ERVFan,0.0);    //关ERV
	APortOut(PTBFan1,0.0);   //关PTB Fan
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
		if(CO2<800)                        //浓度低于600ppm，关闭ERV风扇
		{
			DPortOut(ERVFan,0);
			ERVState=0;
		}
		else if(CO2>=800)                   //浓度高于800ppm，开ERV风扇，开度为30%
		{
			DPortOut(ERVFan,1);		//ERV开30%
			ERVState=1;
		}
}

void RoomTempControl(float value)
{
	if(value>25.0)
		{
			APortOut(PTBFan1,8.0);    //PTB Fan 开80%
			APortOut(PTBFan2,8.0);
		}
	else if(value<24.5)
	{
			APortOut(PTBFan1,0.0);    //PTB Fan 关闭
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

//有人模式
u8 OccupanyMode(void)
{
	u8 alarm=0;
	alarm=NSLH_Alarm();
	if(alarm!=0)
		return alarm;
	
	APortOut(CHWV,ValvePosition);		//PID control valve
	
	if(RoomTemp>=RoomTempSv+5)          //所有数据大小都乘以了10
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

//无人模式
u8 Unoccupanied(void)
{
	u8 alarm=0;
	alarm=NSLH_Alarm();
	if(alarm!=0)
		return alarm;
	
	APortOut(CHWV,ValvePosition);		//PID control valve
	return 0;
}
