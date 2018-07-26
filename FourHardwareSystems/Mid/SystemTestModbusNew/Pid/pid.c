// PID.cpp : Defines the entry point for the console application.
#include "pid.h"

//定义PID参数全局变量结构体及其指针
PID PidPar[4];   //{1,1,1,0.3,0.1,0,0,0,5.0,2.0}
PP pid = PidPar;


///对PID参数初始化
void  PID_Initial()
{
	int i;
  for(i=0;i<4;i++)
  {
		(pid+i)->T = 1;               //默认采样周期为1s
	  (pid+i)->Kp = 1;              //默认比例增益为 1
	  (pid+i)->Ki = 0;              //默认积分增益为 0
	  (pid+i)->Kd = 0;              //默认微分增益为 0
	  (pid+i)->sp = 5.0;            //默认目标值是5.0
	  (pid+i)->last_out = 2.0;      //u0
  }
}

//读取上位机传递来的PID参数并赋值给PID参数全局变量
void PID_SetValue(int ith,float Kp,float Ki, float Kd, float sp)
{
	int i = ith;
	(pid+i)->Kp = Kp;
	(pid+i)->Ki = Ki;
	(pid+i)->Kd = Kd;
	(pid+i)->sp = sp;
}

//采用位置式PID算法，计算执行器的当前给定
float PID_Calcu(int ith,float pv,float maxout)
{
	float out;
  float err;        //当前的偏差 e(n)
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
//从用户输入端的Kp,Ki,Kd推导出 积分时间常数Ti以及微分时间常数Td

void GetTimeConstant()
{
    pid->Ti = pid->Kp * pid->T / pid->Ki;
    pid->Td = pid->Kd * pid->T / pid->Kp;
}
*/

