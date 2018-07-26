#ifndef _PID_H
#define _PID_H

#include <stdio.h>
#include <stdlib.h>

//定义PID结构体--PID_PAR 数据类型 *PP指向结构体的指针
typedef struct _PID
{
  int Ti;
  int Td;
  int T;
  float Kp;
  float Ki;
  float Kd;
  float last_err;
  float prev_err;
  float sp;
  float last_out;
}PID,*PP;
extern PID PidPar[4];

void  PID_Initial(void);
float PID_Calcu(int ith,float pv,float maxout);
void PID_SetValue(int ith,float Kp,float Ki, float Kd,float sp);

#endif /*_PID_*/
