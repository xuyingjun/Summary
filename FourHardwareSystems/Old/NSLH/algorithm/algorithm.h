#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#include "logic.h"

#define Number 8

extern float Kp;               //比例系数
extern float Ki;               //积分系数
extern float Kd;               //微分系数

extern float p;
extern float i;
extern float d;

float PID_ValveControl(float sv,float pv);      //增量式PID
float PID_FanControl(float sv,float pv);
float AverageFilter(float data);                //递推平均滤波算法1
float recursive_average_filter(float data);     //递推平均滤波算法2

#endif
