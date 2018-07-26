#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#include "logic.h"

#define Number 8

extern float Kp;               //����ϵ��
extern float Ki;               //����ϵ��
extern float Kd;               //΢��ϵ��

extern float p;
extern float i;
extern float d;

float PID_ValveControl(float sv,float pv);      //����ʽPID
float PID_FanControl(float sv,float pv);
float AverageFilter(float data);                //����ƽ���˲��㷨1
float recursive_average_filter(float data);     //����ƽ���˲��㷨2

#endif
