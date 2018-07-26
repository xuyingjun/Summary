#include "algorithm.h"

float PID_ValveControl(float sv,float pv)
{
	float Output=0.0;            //PID当前输出
	float last_Output=0.0;       //上一次输出
	float ek=0.0;                //当次误差
	float ek1=0.0;               //上次误差
	float ek2=0.0;               //上两次误差
	float pError=0.0;            //比例误差
	float iError=0.0;            //积分误差
	float dError=0.0;            //微分误差
	
	Output=ADC_data[CHWVfb];     //读取当前水阀反馈值
	
	ek=sv-pv;
	pError=ek-ek1;
	iError=ek;
	dError=ek-2*ek1+ek2;
	
	ek2=ek1;                 //储存前两次误差
	ek1=ek;                  //储存前一次误差
	
	Output=p*pError+i*iError+d*dError+last_Output;      //获取PID调节的输出值
	last_Output=Output;      //存储前一次输出值
	
	return Output;
}

float PID_FanControl(float sv,float pv)
{
	float FanOutput=0.0;            //PID当前输出
	float last_Output=0.0;       //上一次输出
	float ek=0.0;                //当次误差
	float ek1=0.0;               //上次误差
	float ek2=0.0;               //上两次误差
	float pError=0.0;            //比例误差
	float iError=0.0;            //积分误差
	float dError=0.0;            //微分误差
	
	FanOutput=DAC_data[0];     //读取当前风扇值
	
	ek=sv-pv;
	pError=ek-ek1;
	iError=ek;
	dError=ek-2*ek1+ek2;
	
	ek2=ek1;                 //储存前两次误差
	ek1=ek;                  //储存前一次误差
	
	FanOutput = Kp*pError+Ki*iError+Kd*dError+last_Output;      //获取PID调节的输出值
	last_Output=FanOutput;      //存储前一次输出值
	
	return FanOutput;
}

//递推平均滤波算法
float AverageFilter(float data)
{
	int counter;
	float sum=0.0;
	float average=0.0;
	float ValueBuf[Number];
	int count=0;
	
	ValueBuf[count++]=data;
	if(count==Number)                       //先进先出，再求平均值
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

