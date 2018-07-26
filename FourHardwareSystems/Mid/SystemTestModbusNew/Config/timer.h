#ifndef _TIMER_H
#define _TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"

void InitializeTimer(int n,float time);
void TurnOnTim(int x);
void TurnOffTim(int x);
void Tim2Configure(void);

#endif
