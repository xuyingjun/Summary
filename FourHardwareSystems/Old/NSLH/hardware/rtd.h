#ifndef RTD_H
#define RTD_H

#include "stm32f10x.h"
#include <stdio.h>

void RTD_init(void);
void RTD_onechannel(int channel,float* temperature);
void RTD_allchannels(float* temperature);

#endif
