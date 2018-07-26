#ifndef _DAC8554_H
#define _DAC8554_H

#define REF  5   //��׼��ѹ��
 
 
/*
		���IO�ڵĺ궨��
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
  
/*���IO������*/
void DAC7554_GPIO_Config(void);

/*��Ƭ����DAC����24bit�����ݣ�ע�⣺��MDK for STM32�У�int����32bit  ������16λ����int��16λ���˳�����ֱ����*/
void DAC8554_Send_24_bits(unsigned int data);

void DAC8554_delay(void);
void SetAoOut(float Parm,u8 channel,u8 CurrentFlag);
#endif
