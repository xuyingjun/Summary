// PID.cpp : Defines the entry point for the console application.
#include "pid.h"

//����PID����ȫ�ֱ����ṹ�弰��ָ��
PID PidPar[4];   //{1,1,1,0.3,0.1,0,0,0,5.0,2.0}
PP pid = PidPar;


///��PID������ʼ��
void  PID_Initial()
{
	int i;
  for(i=0;i<4;i++)
  {
		(pid+i)->T = 1;               //Ĭ�ϲ�������Ϊ1s
	  (pid+i)->Kp = 1;              //Ĭ�ϱ�������Ϊ 1
	  (pid+i)->Ki = 0;              //Ĭ�ϻ�������Ϊ 0
	  (pid+i)->Kd = 0;              //Ĭ��΢������Ϊ 0
	  (pid+i)->sp = 5.0;            //Ĭ��Ŀ��ֵ��5.0
	  (pid+i)->last_out = 2.0;      //u0
  }
}

//��ȡ��λ����������PID��������ֵ��PID����ȫ�ֱ���
void PID_SetValue(int ith,float Kp,float Ki, float Kd, float sp)
{
	int i = ith;
	(pid+i)->Kp = Kp;
	(pid+i)->Ki = Ki;
	(pid+i)->Kd = Kd;
	(pid+i)->sp = sp;
}

//����λ��ʽPID�㷨������ִ�����ĵ�ǰ����
float PID_Calcu(int ith,float pv,float maxout)
{
	float out;
  float err;        //��ǰ��ƫ�� e(n)
	float term1,term2,term3;
	int i = ith;

	err = (pid+i)->sp - pv;  //e(n)
	term1 = (pid+i)->Kp * (err - (pid+i)->last_err);
	term2 = (pid+i)->Ki * err;
	term3 = (pid+i)->Kd * (err + (pid+i)->prev_err - 2 * (pid+i)->last_err);
	out = (pid+i)->last_out + term1 + term2 + term3;
	if(out >= maxout) 
		{
			out = maxout;
	    err = 0;
	  }
		
	(pid+i)->prev_err = (pid+i)->last_err; 
	(pid+i)->last_err = err;
	(pid+i)->last_out = out;
	
	return out;
}

/*
//���û�����˵�Kp,Ki,Kd�Ƶ��� ����ʱ�䳣��Ti�Լ�΢��ʱ�䳣��Td

void GetTimeConstant()
{
    pid->Ti = pid->Kp * pid->T / pid->Ki;
    pid->Td = pid->Kd * pid->T / pid->Kp;
}
*/

