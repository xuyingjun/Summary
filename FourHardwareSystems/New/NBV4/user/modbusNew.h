#ifndef __MODBUSNEW_H
#define __MODBUSNEW_H

#include "stm32f10x.h"

//定义相关函数
void Modbus_RegMap(void);
void RS485_Service(void);
void Modbus_02_Solve(void);
void Modbus_01_Solve(void);
void Modbus_05_Solve(void);
void Modbus_15_Solve(void);
void Modbus_03_Solve(void);
void Modbus_06_Solve(void);
void Modbus_16_Solve(void);

#endif
