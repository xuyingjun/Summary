#ifndef _DAC8554_H
#define _DAC8554_H

#define REF  5   //基准电压的
 
 
/*
		相关IO口的宏定义
*/
#define SYNC_Group  GPIOD
#define SYNC_Pin    GPIO_Pin_0
#define SCLK_Group  GPIOD
#define SCLK_Pin    GPIO_Pin_1
#define DIN_Group   GPIOD
#define DIN_Pin     GPIO_Pin_2
#define LDAC_Pin     GPIO_Pin_3

#define  SYNC_1()  	GPIO_WriteBit(SYNC_Group, SYNC_Pin,Bit_SET)  
#define  SYNC_0()   GPIO_WriteBit(SYNC_Group, SYNC_Pin,Bit_RESET)  
#define  SCLK_1()   GPIO_WriteBit(SCLK_Group, SCLK_Pin,Bit_SET) 
#define  SCLK_0()  	GPIO_WriteBit(SCLK_Group, SCLK_Pin,Bit_RESET)
#define  DIN_1()  	GPIO_WriteBit(DIN_Group, DIN_Pin,Bit_SET)   
#define  DIN_0()  	GPIO_WriteBit(DIN_Group, DIN_Pin,Bit_RESET)
  
/*相关IO口配置*/
void DAC7554_GPIO_Config(void);

/*单片机向DAC发送24bit的数据，注意：在MDK for STM32中，int型是32bit  在其它16位机中int是16位，此程序不能直接用*/
void DAC8554_Send_24_bits(unsigned int data);

void DAC8554_delay(void);
void SetAoOut(float Parm,u8 channel,u8 CurrentFlag);
#endif
