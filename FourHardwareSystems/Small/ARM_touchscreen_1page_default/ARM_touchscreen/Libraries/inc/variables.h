#ifndef _VARIABLES_H
#define _VARIABLES_H


#include "stm32f10x.h"
#include <stdio.h>


 


typedef struct
{
	u16 pv; /*integer that contains the process value*/
	u16 sp; /*integer that contains the set point*/
	float integral;
	float pgain;
	float igain;
	float dgain;
	u16 deadband;
	u16 last_error;
}_pid; 

typedef struct
{
   float p;
   float i;
   float d;
}_pid_data;	
#endif

