#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f10x.h"
#include "Globle.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!


//IO��������
#define SDA_IN()  {GPIOG->CRL&=0XFFFFF0FF;GPIOG->CRL|=8<<8;}
#define SDA_OUT() {GPIOG->CRL&=0XFFFFF0FF;GPIOG->CRL|=3<<8;}

//IO��������	 
#define IIC_SCL    PGout(3) //SCL
#define IIC_SDA    PGout(2) //SDA	 
#define READ_SDA   PGin(2)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















