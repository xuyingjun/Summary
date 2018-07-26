#ifndef _MUXCFG_H_
#define _MUXCFG_H_
#include "stm32f10x.h"

//定义12路AI输入相应的管脚
#define Ai1Mux_Port   GPIOF
#define Ai1Mux_Pin    GPIO_Pin_0
#define Ai2Mux_Port   GPIOF
#define Ai2Mux_Pin    GPIO_Pin_1
#define Ai3Mux_Port   GPIOF
#define Ai3Mux_Pin    GPIO_Pin_2
#define Ai4Mux_Port   GPIOF
#define Ai4Mux_Pin    GPIO_Pin_3
#define Ai5Mux_Port   GPIOF
#define Ai5Mux_Pin    GPIO_Pin_4
#define Ai6Mux_Port   GPIOF
#define Ai6Mux_Pin    GPIO_Pin_5
#define Ai7Mux_Port   GPIOF
#define Ai7Mux_Pin    GPIO_Pin_6
#define Ai8Mux_Port   GPIOF
#define Ai8Mux_Pin    GPIO_Pin_7

#define Ai9Mux_Port   GPIOE
#define Ai9Mux_Pin    GPIO_Pin_2
#define Ai10Mux_Port  GPIOE
#define Ai10Mux_Pin   GPIO_Pin_3
#define Ai11Mux_Port  GPIOE
#define Ai11Mux_Pin   GPIO_Pin_4
#define Ai12Mux_Port  GPIOE
#define Ai12Mux_Pin   GPIO_Pin_5

//定义8路AO输出管脚
#define Ao1Mux_Port   GPIOA
#define Ao1Mux_Pin    GPIO_Pin_0
#define Ao2Mux_Port   GPIOA
#define Ao2Mux_Pin    GPIO_Pin_1
#define Ao3Mux_Port   GPIOA
#define Ao3Mux_Pin    GPIO_Pin_2
#define Ao4Mux_Port   GPIOA
#define Ao4Mux_Pin    GPIO_Pin_3
#define Ao5Mux_Port   GPIOA
#define Ao5Mux_Pin    GPIO_Pin_4
#define Ao6Mux_Port   GPIOA
#define Ao6Mux_Pin    GPIO_Pin_5
#define Ao7Mux_Port   GPIOA
#define Ao7Mux_Pin    GPIO_Pin_6
#define Ao8Mux_Port   GPIOA
#define Ao8Mux_Pin    GPIO_Pin_7

//定义输入输出 电压电流信号
#define MUX_CURRENT 0
#define MUX_VOLTAGE 1

//定义相关函数
void AiChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent);        //AI电流电压输入类型
void AoChannelSetMux(u8 Channel,u8 MuxVoltageOrCurrent);        //AO电流电压输出类型
void MuxGpioCfg(void);                                          //端口管脚配置
 
#endif




