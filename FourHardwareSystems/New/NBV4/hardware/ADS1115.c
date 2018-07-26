#include "stm32f10x.h"
#include "math.h"
#include "systick_timer.h"
#include "myiic.h"
#include "stm32f10x_i2c.h"
unsigned char BUF[8]; 
int16_t resault=0;
int   valu_stm32ads1115_1;
float s1,s2;

void I2C_WriteByte(unsigned char id,unsigned char write_address,unsigned char byte)
{
	IIC_Start();
	//������ʼ����
	IIC_Send_Byte(id);
	//���豸�����豸��ַ
	while(IIC_Wait_Ack());
	//�ȴ�ACK
	IIC_Send_Byte(write_address);
	//�Ĵ�����ַ
	while(IIC_Wait_Ack());
	//�ȴ�ACK
	IIC_Send_Byte(byte);
	//��������
	while(IIC_Wait_Ack());
	//�������
	IIC_Stop();
	//���������ź�
}
//---------------------------------------------------------//
void AD_init(unsigned char id,unsigned char write_address,unsigned char byte1,unsigned char byte2)
{
	IIC_Start();
	//������ʼ����
	IIC_Send_Byte(id);
	//���豸�����豸��ַ
	while( IIC_Wait_Ack());
	//�ȴ�ACK
	IIC_Send_Byte(write_address) ;//���üĴ���
	//�Ĵ�����ַ
	while( IIC_Wait_Ack());
	delay_us(4);
	//�ȴ�ACK
	IIC_Send_Byte(byte1) ;//c-an0 d-an1 e-an2 f-an3  0->6.144 2->4.096,���ֽ�����
	//��������
	while(IIC_Wait_Ack());
	//�������
	IIC_Send_Byte(byte2);//���ֽ� ���ת���ٶ�
	while(IIC_Wait_Ack());
	//�������
	IIC_Stop();
	//���������ź�
}
//----------------------------------------------------//
void I2C_ADpoint(unsigned char id,unsigned char write_address)
{
	IIC_Start();
	//������ʼ����
	IIC_Send_Byte(id);
	//���豸�����豸��ַ
	while(IIC_Wait_Ack());
	//�ȴ�ACK
	IIC_Send_Byte(write_address);
	//�Ĵ�����ַ
	while(IIC_Wait_Ack());

	IIC_Stop();
	//���������ź�
}







//-------------------------------------------------------//

unsigned char I2C_ReadByte(unsigned char  id, unsigned char read_address)
{  
	unsigned char temp; 	

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  	//�ȴ�I2C
  	I2C_GenerateSTART(I2C1, ENABLE);
  	//������ʼ�ź�
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    //EV5
  	I2C_Send7bitAddress(I2C1, id, I2C_Direction_Transmitter);
	//���͵�ַ
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  	//EV6
  	I2C_Cmd(I2C1, ENABLE);
 	//�������ÿ������EV6
  	I2C_SendData(I2C1, read_address);  
	//���Ͷ���ַ
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  	//EV8 
  	I2C_GenerateSTART(I2C1, ENABLE);
	//���·���
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  	//EV5
  	I2C_Send7bitAddress(I2C1, id, I2C_Direction_Receiver);
  	//���Ͷ���ַ
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  	//EV6  
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
	//�ر�Ӧ���ֹͣ��������
    while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
	      
    temp = I2C_ReceiveData(I2C1);
   
  	I2C_AcknowledgeConfig(I2C1, ENABLE);
		
    return temp;
}

//----------------------------------------------------------//
void I2C_Read2(unsigned char  id, unsigned char read_address)
{  
	
  	IIC_Start();
  	//������ʼ�ź�
  	IIC_Send_Byte(id);
	//���͵�ַ
  	while(IIC_Wait_Ack());
	//�ȴ�ACK
  	//IIC_Send_Byte(read_address);
	//���Ͷ��õ�ַ
  	//while(IIC_Wait_Ack());
	
  	 BUF[1] = IIC_Read_Byte(1);
	//���·���
  	delay_us(5);
	BUF[2] = IIC_Read_Byte(1);
	IIC_Stop();
		
    
		
}
float ReadAdOfChannel(unsigned char  Channel,unsigned char CurrentFlag)
{
	float resaultVoltageOrCurrent=0;
	
	
	int16_t CigH=0xc2;
	switch(CurrentFlag)
	{
		case 0:CigH=0xce;break;  //FS=+-0.256
		case 1:CigH=0xc2;break;  //FS=+-4.096
		default:break;
	}

	switch(Channel)
	{
		case 1:AD_init(0x90,0x01,CigH|0x00,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x90,0x00);
					 delay_ms(2);
		       I2C_Read2(0x91,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 2:AD_init(0x90,0x01,CigH|0x10,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x90,0x00);
					 delay_ms(2);
		       I2C_Read2(0x91,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 3:AD_init(0x90,0x01,CigH|0x20,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x90,0x00);
					 delay_ms(2);
		       I2C_Read2(0x91,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
	  case 4:AD_init(0x90,0x01,CigH|0x30,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x90,0x00);
					 delay_ms(2);
		       I2C_Read2(0x91,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 5:AD_init(0x92,0x01,CigH|0x00,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x92,0x00);
					 delay_ms(2);
		       I2C_Read2(0x93,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 6:AD_init(0x92,0x01,CigH|0x10,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x92,0x00);
					 delay_ms(2);
		       I2C_Read2(0x93,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 7:AD_init(0x92,0x01,CigH|0x20,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x92,0x00);
					 delay_ms(2);
		       I2C_Read2(0x93,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
	  case 8:AD_init(0x92,0x01,CigH|0x30,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x92,0x00);
					 delay_ms(2);
		       I2C_Read2(0x93,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 9:AD_init(0x94,0x01,CigH|0x00,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x94,0x00);
					 delay_ms(2);
		       I2C_Read2(0x95,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 10:AD_init(0x94,0x01,CigH|0x10,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x94,0x00);
					 delay_ms(2);
		       I2C_Read2(0x95,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		case 11:AD_init(0x94,0x01,CigH|0x20,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x94,0x00);
					 delay_ms(2);
		       I2C_Read2(0x95,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
	  case 12:AD_init(0x94,0x01,CigH|0x30,0xe2);
		       delay_ms(2);
					 I2C_ADpoint(0x94,0x00);
					 delay_ms(2);
		       I2C_Read2(0x95,0x00);
		       resault = (BUF[1] << 8) | BUF[2];
					 break;
		default:break;
	}
	switch(CurrentFlag)
	{ 
		case 0: resaultVoltageOrCurrent=0.256*resault/32767.0*51;break;    //FS=+-0.256
		case 1: resaultVoltageOrCurrent=4.096*resault/32767.0/200.0*1000;break;//FS=+-4.096
		default:resaultVoltageOrCurrent=0;break;
	}
	return resaultVoltageOrCurrent;
}
//-----------------------------------------------//
void read_ADS1115_1()
{
	 /*******************************AD1***********************************************/
	   AD_init(0x90,0x01,0xc2,0xe2);
	   //������ַ90�����üĴ���01���Ĵ�����λ���Ĵ�����λe2
	   //���ֽ����ã�c-an0 d-an1 e-an2 f-an3 ; 0->6.144 2->4.096 4->2.048
	 delay_ms(1);	
	   I2C_ADpoint(0x90,0x00);
     delay_ms(2);	
     I2C_Read2(0x91,0x00);
	 valu_stm32ads1115_1 = (BUF[1] << 8) | BUF[2]; 
	 if(valu_stm32ads1115_1>32768)valu_stm32ads1115_1=valu_stm32ads1115_1-65536;
     s1 = ((float)valu_stm32ads1115_1/(float)0x7fff) *4.096*10.02/4.12;                //S1 AI1 ͨ����ѹֵ
    // if(valu_stm32ads1115_1>0x7fff)valu_stm32ads1115_1-=0xffff;	 
	
	/********************************AD2**********************************************/
	AD_init(0x90,0x01,0xd2,0xe2);
	   //������ַ90�����üĴ���01���Ĵ�����λ���Ĵ�����λe2
	   //���ֽ����ã�c-an0 d-an1 e-an2 f-an3 ; 0->6.144 2->4.096 4->2.048
	 delay_ms(1);	
	   I2C_ADpoint(0x90,0x00);
     delay_ms(2);	
     I2C_Read2(0x91,0x00);
	 valu_stm32ads1115_1 = (BUF[1] << 8) | BUF[2]; 
	 if(valu_stm32ads1115_1>32768)valu_stm32ads1115_1=valu_stm32ads1115_1-65536;
     s2 = ((float)valu_stm32ads1115_1/(float)0x7fff) *4.096*10.02/4.12;                 //S2 AI2 ͨ����ѹֵ
    // if(valu_stm32ads1115_1>0x7fff)valu_stm32ads1115_1-=0xffff;	
}

void stm32ads1115_1_init(void)
{
	IIC_Init();
}





