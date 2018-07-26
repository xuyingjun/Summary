#ifndef _MODBUS_H
#define _MODBUS_H

#include "stdint.h"
#include "usart.h"
#include "Globle.h"

#define modbus_deviceId  0x05
#define RS485MOED_R  GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define RS485MOED_S  GPIO_ResetBits(GPIOA,GPIO_Pin_11)

void DealModBus(void);
u16 crc16(u8 *ptr,u8 len);
void FloatPut(float Fdata,unsigned char *Buf,unsigned char Pos);
void FloatGet(float Fdata,u8 *Buf,u8 Pos);

#endif
