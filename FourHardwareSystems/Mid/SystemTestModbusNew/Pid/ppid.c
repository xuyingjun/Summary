#include "stdio.h"
#include "stdlib.h"

//����ʽPID����
float ExeValue=0.0;

struct _pid
{
	float SetValue;          //�趨ֵ
	float ActualValue;    //ʵ��ֵ
	float err;                    //���ֵ
	float err_last;           //����һ��ƫ��
	float kp, ki, kd;        //PID����
	float integral;          //������
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