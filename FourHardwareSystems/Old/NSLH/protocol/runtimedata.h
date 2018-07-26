#ifndef _RUNTIMEDATA_H
#define _RUNTIMEDATA_H

#include <stdio.h>
#include "adc.h"
#include "rtd.h"
#include "dac.h" 	   			 
#include "digital_io.h"	
#include "protocol.h"
#include "pid.h"
#include "logic.h"
#include "touchscreen.h"

//data---电信号值，value---物理意义值
extern float  Registers[16];                 //寄存器值,存储非端口类数据
extern float  ADC_value[12];                 //ADC物理意义值
extern float  ADC_data[12];                  //ADC电信号值
extern float  RTD_value[4];
extern float  DAC_value[8];
extern float  DAC_data[8];
extern u8   	 DI_data[8];
extern u8     DO_data[8];

extern float dataTemp[12];
extern float RTDtemperature[4];
	
extern LOOPSEL AOAM[8];
extern LOOPSEL REGAM[16];
extern PARAMS AOParams[8];
extern PARAMS AIParams[12];  //y = a*x +b

extern char ReadPortsBuffer[165];
extern char WritePortsBuffer[104];
extern char ReadPIDBuffer[96];
extern char WritePIDBuffer[97]; //HandleBit(1),PID1---PID4(24/each);

void DataProcess(void);
void readPorts(char* readPortsBuffer);
void readPID(char* readPIDBuffer);
void writePorts(char* writePortsBuffer);
void writePID(char* writePIDBuffer);
float getAIPhysicalValue(float eletricData,PARAMS par);
float getAOPhysicalValue(float eletricData,PARAMS par);
#endif /*RUNTIMEDATA_H*/
