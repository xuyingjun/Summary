#ifndef __LOGIC_H
#define __LOGIC_H

#include "stm32f10x.h"
#include "rtd.h"
#include "ads1115.h"
#include "systick_timer.h"	
#include "DAC8554.h"
#include "MuxCfg.h"
#include "Dio.h"
#include "Pwm.h"
#include "Globle.h"
#include <stdlib.h>

extern float AportIn[12];
extern float AportOut[8];
extern u8 DigitalIn;
extern u8 DigitalOut[8];


void APortIn(u8 channel);
void APortOut(u8 channel, float value);
u8 DPortIn(void);
void DPortOut(u8 channel, u8 value);

#endif
