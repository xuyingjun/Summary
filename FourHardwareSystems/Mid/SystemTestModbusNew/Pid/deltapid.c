#include "stdio.h"
#include "stdlib.h"

//增量式PID代码
float ExeValue=0.0;

struct _pid
{
	float SetValue;          //设定值
	float ActualValue;    //实际值
	float err;                    //误差值
	float err_next;          //上一个偏差
	float err_last;           //再上一个偏差
	float kp, ki, kd;       //PID参数
} pid;

void PID_init()
{
	pid.SetValue=0.0;
	pid.ActualValue=0.0;
	pid.err=0.0;
	pid.err_next=0.0;
	pid.err_last=0.0;
	pid.kp=0.0;
	pid.ki=0.0;
	pid.kd=0.0;
}

float PID_Control(float setvalue)
{
	float IncrementValue=0.0;
	pid.SetValue = setvalue;
	pid.err = pid.SetValue - pid.ActualValue;
	IncrementValue = pid.kp*(pid.err-pid.err_next)+pid.ki*pid.err+pid.kd*(pid.err-2*pid.err_next+pid.err_last);
	pid.ActualValue+=IncrementValue;
	pid.err_last=pid.err_next;
	pid.err_next=pid.err;
	return pid.ActualValue;
}

//int main(void)
//{
//	PID_init();
//	ExeValue= PID_Control(1.0);
//}
