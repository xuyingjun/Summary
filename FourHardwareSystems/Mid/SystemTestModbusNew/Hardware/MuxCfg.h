#ifndef _MUXCFG_H_
#define _MUXCFG_H_
#include "stm32f10x.h"
#define Ai1Mux_Port   GPIOE
#define Ai1Mux_Pin    GPIO_Pin_3
#define Ai2Mux_Port   GPIOE
#define Ai2Mux_Pin    GPIO_Pin_4
#define Ai3Mux_Port   GPIOE
#define Ai3Mux_Pin    GPIO_Pin_5
#define Ai4Mux_Port   GPIOE
#define Ai4Mux_Pin    GPIO_Pin_6





#define Ao1Mux_Port   GPIOD
#define Ao1Mux_Pin    GPIO_Pin_4
#define Ao2Mux_Port   GPIOD
#define Ao2Mux_Pin    GPIO_Pin_5
#define Ao3Mux_Port   GPIOD
#define Ao3Mux_Pin    GPIO_Pin_6
#define Ao4Mux_Port   GPIOD
#define Ao4Mux_Pin    GPIO_Pin_7


#define MUX_CURRENT 0
#define MUX_VOLTAGE 1



void AiChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent);
void AoChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent);
void MuxGpioCfg(void);
#endif




