#include "algorithm.h"

float PID_ValveControl(float sv,float pv)
{
	float Output=0.0;            //PID��ǰ���
	float last_Output=0.0;       //��һ�����
	float ek=0.0;                //�������
	float ek1=0.0;               //�ϴ����
	float ek2=0.0;               //���������
	float pError=0.0;            //�������
	float iError=0.0;            //�������
	float dError=0.0;            //΢�����
	
	Output=ADC_data[CHWVfb];     //��ȡ��ǰˮ������ֵ
	
	ek=sv-pv;
	pError=ek-ek1;
	iError=ek;
	dError=ek-2*ek1+ek2;
	
	ek2=ek1;                 //����ǰ�������
	ek1=ek;                  //����ǰһ�����
	
	Output=p*pError+i*iError+d*dError+last_Output;      //��ȡPID���ڵ����ֵ
	last_Output=Output;      //�洢ǰһ�����ֵ
	
	return Output;
}

float PID_FanControl(float sv,float pv)
{
	float FanOutput=0.0;            //PID��ǰ���
	float last_Output=0.0;       //��һ�����
	float ek=0.0;                //�������
	float ek1=0.0;               //�ϴ����
	float ek2=0.0;               //���������
	float pError=0.0;            //�������
	float iError=0.0;            //�������
	float dError=0.0;            //΢�����
	
	FanOutput=DAC_data[0];     //��ȡ��ǰ����ֵ
	
	ek=sv-pv;
	pError=ek-ek1;
	iError=ek;
	dError=ek-2*ek1+ek2;
	
	ek2=ek1;                 //����ǰ�������
	ek1=ek;                  //����ǰһ�����
	
	FanOutput = Kp*pError+Ki*iError+Kd*dError+last_Output;      //��ȡPID���ڵ����ֵ
	last_Output=FanOutput;      //�洢ǰһ�����ֵ
	
	return FanOutput;
}

//����ƽ���˲��㷨
float AverageFilter(float data)
{
	int counter;
	float sum=0.0;
	float average=0.0;
	float ValueBuf[Number];
	int count=0;
	
	ValueBuf[count++]=data;
	if(count==Number)                       //�Ƚ��ȳ�������ƽ��ֵ
		count=0;
	for(counter=0;counter<Number;counter++)
	{
		sum+=ValueBuf[counter];
	}
	average=sum/Number;
	return average;
}

float recursive_average_filter(float data)
{
	float sum=0.0;
	float average=0.0;
	int i;
	float valuebuf[9];
	
	valuebuf[Number]=data;
	for(i=0;i<Number;i++)
	{
		valuebuf[i]=valuebuf[i+1];
		sum+=valuebuf[i];
	}
	average=sum/Number;
	
	return average;
}

