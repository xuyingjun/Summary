#include "stdio.h"
#include "stdlib.h"

//增量式PID代码
float ExeValue=0.0;

struct _pid
{
	float SetValue;          //设定值
	float ActualValue;    //实际值
	float err;                    //误差值
	float err_last;           //再上一个偏差
	float kp, ki, kd;        //PID参数
	float integral;          //积分项
} pid;

void PID_init()
{
	pid.SetValue=0.0;
	pid.ActualValue=0.0;
	pid.err=0.0;
	pid.err_last=0.0;
	kp=0.0;
	ki=0.0;
	kd=0.0;
	pid.integral=0.0;
}

float PID_Control(float setvalue)
{
	pid.SetVaule = setvalue;
	pid.err = pid.SetValue - pid.ActualValue;
	pid.integral += pid.err;
	pid.AcutalValue = pid.kp*pid.err + pid.ki*pid.integral + pid.kd*(pid.err-pid.err_last);
	pid.err_last=pid.err;
	return pid.AcutualValue;
}

int main(void)
{
	PID_init();
	ExeValue= PID_Control(1.0);
}