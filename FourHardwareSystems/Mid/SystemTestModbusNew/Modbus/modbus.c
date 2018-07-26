#include "modbus.h"
//#include "winsock2.h"

u16 SumRec=0;//���յ���У���
u16 SumCal=0;//�����У���

void DealModBus(void)
{
	u8 REG0X03[100];    //������0x03��Ӧ�ļĴ�����ַΪ0x0001-0x00064  
	u16 REG0X06[100];    //������0x06��Ӧ�ļĴ�����ַΪ0x0001-0x00064  
	
	u16 DatLength=0;     //���ݳ���
	u16 StartRegAdd=0;   //��ʼ��ַ
	
	u8 i,sendLen;        //��ر���
//	float temp=0.0;
	if(Uart1_receive_buffer[0]!=modbus_deviceId)  //�˶Ա�����ַ
	{
		return; //��ַ����
	}
	
	SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);//�����У���
	SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];  //����Ĭ�Ͻ��յ���У���Ҳ�ǵͰ�λ��ǰ���߰�λ�ں�
	if(SumCal!=SumRec)           //У��Ͳ�һ��
	{         
		return ;                   //У��ʹ���
	}
	
	//�����ݴ浽���Ĵ����У���ַΪ0x01-0x64
//	temp=advalue[1]*1000.0;
//	REG0X03[0]=*(uint32_t *)(&temperature)/65536;
//	REG0X03[1]=*(uint32_t *)(&temperature)%65536;
	//FloatPut(temperature,REG0X03,0);
	//FloatGet(temperature,REG0X03,4);
	for(i=0;i<100;i++)
	{
		REG0X03[i]=ReadReg[i];
	}
	
	//�����ݴ浽д�Ĵ����У���ַΪ0x01-0x64
	for(i=0;i<100;i++)
	{
		REG0X06[i]=WriteReg[i];
	}
	
	//0---Device Id 1---funciton code  2,3---�Ĵ�����ʼ��ַ   4,5---�Ĵ�����ַ����
	StartRegAdd=Uart1_receive_buffer[2]*256+Uart1_receive_buffer[3];   //������ʵ�Ĵ�����ַ
	DatLength=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];     //����Ĵ�����ַ����
	
	//�жϳ�ʼ��ַ�Ƿ��������Ļ�������Ӧ������Ӧ����Ϣ
	if((StartRegAdd+DatLength)<0x64)
	{
		sendLen=0;
		Uart1_Send_buffer[sendLen++]=modbus_deviceId;                  //�豸ID
		Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[1];          //��Ӧ������
		
		switch(Uart1_receive_buffer[1])     //�����룬���ݲ�ͬ�Ĺ����룬���ض�Ӧ��Ӧ����Ϣ
		{
			case 0x03:
							if((StartRegAdd+DatLength)>100)return;
							Uart1_Send_buffer[sendLen++]=DatLength*2;    //ÿ���Ĵ���ֵ�洢��2���ֽڣ��������ݳ���Ϊ2*N(�Ĵ�������)
							for(i=0;i<DatLength;i++)                     //��ȡ�������߶���Ĵ���
							{
								Uart1_Send_buffer[sendLen++] = (uint8_t)REG0X03[StartRegAdd+i]>>8;        //��ֵ���ֽ�
								Uart1_Send_buffer[sendLen++]=(uint8_t)(REG0X03[StartRegAdd+i]&0xFF);      //��ֵ���ֽ�
							}
			        break;
			case 0x06:						                                //д�����Ĵ���
							if(StartRegAdd>=100)return;                 //д�Ĵ����ĵ�ַ����100���򷵻�
							
							REG0X06[StartRegAdd]=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5]; //����ҪЩ�����ݴ洢��ָ���ĵ�ַ��
							
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];  //������Ӧ�ļĴ�����ַ������
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
			case 0x10:
							if(StartRegAdd+DatLength>=100)return;                       //д����Ĵ���������ַ��Χ
							for(i=0;i<DatLength;i++)
							{
								REG0X06[StartRegAdd+i]=Uart1_receive_buffer[7+i*2]*256+Uart1_receive_buffer[8+i*2];
							}
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];  //������Ӧ�ļĴ�����ַ������
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
							
			default:                           //��֧�ֵĹ�����
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							Uart1_Send_buffer[1]=0x80;                             //���������λ��1
							Uart1_Send_buffer[2]=0x01;                             //�����쳣��Ϊ01-��Ч����
							break; 
		}
	
		//��������У���
		SumCal=crc16(&Uart1_Send_buffer[0],sendLen);            //�����У���
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal>>8);      //crc���ֽ�
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal&0xff);    //crc���ֽ�
	}
	
	RS485MOED_S;                                            //485���
  zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);     //������Ӧ֡
  delay_nms(2);
	RS485MOED_R;                                            //485����
	
	for(i=0;i<100;i++)
	{
		WriteReg[i]=REG0X06[i];	
	}
}

/*ModBus ����CRC��Ĳ���Ϊ��

(1).Ԥ��16λ�Ĵ���ΪFFFFH���ƴ˼Ĵ���ΪCRC�Ĵ�����

(2).�ѵ�һ��8λ������CRC�Ĵ����ĵ�λ����򣬰ѽ������CRC�Ĵ�����

(3).�ѼĴ�������������һλ(����λ)����0����λ��������λ��

(4).������λΪ0���ظ���3��(�ٴ���λ)������λΪ1��CRC�Ĵ��������ʽA001��1010 0000 0000 0001���������

(5).�ظ�����3��4��ֱ������8�Σ���������8λ����ȫ�������˴���

(6).�ظ�����2������5��������һ��8λ���ݵĴ���

(7).���õ���CRC�Ĵ�����ΪCRC�롣(CRC�� = CRC_L +CRC_H)

*/
/*****************crcУ��**********************************************/

//crc���ɺ���������crc�洢��Ԥ�ȶ��ڵ�������

//���÷�ʽcrc16��ָ�����ݵ�ָ�룬��ҪУ������ݳ��ȣ�
u16 crc16(u8 *ptr,u8 len)
{
	unsigned long wcrc=0XFFFF;    //Ԥ��16λcrc�Ĵ�������ֵȫ��Ϊ1
	int i=0,j=0;                  //�������
	for(i=0;i<len;i++)            //ѭ������ÿ������
		{
			wcrc^=*ptr++;             //����λ������crc�Ĵ������.ָ���ַ���ӣ�ָ���¸�����
			for(j=0;j<8;j++)          //ѭ���������ݵ�
				{
					if(wcrc&0X0001)       //�ж����Ƴ����ǲ���1�������1�������ʽ�������
						{
							wcrc=wcrc>>1^0XA001;  //�Ƚ���������һλ��������Ķ���ʽ�������
						}
					else                      //�������1����ֱ���Ƴ�
						{
							wcrc>>=1;             //ֱ���Ƴ�
						}
				}
		}
	return wcrc<<8|wcrc>>8;           //�Ͱ�λ��ǰ���߰�λ�ں�
}

//��float���͵����ݴ����uchar���͵�����Buf�У��洢��Buf�ĵ�Pos���ֽڿ�ʼ�������ĸ���ַ
void FloatPut(float Fdata,unsigned char *Buf,unsigned char Pos)
{
	unsigned char *p;
	p=(unsigned char *)&Fdata;
	Buf[Pos] = *p;
	Buf[Pos+1] = *(p+1);
	Buf[Pos+2] = *(p+2);
	Buf[Pos+3] = *(p+3);
}

void FloatGet(float Fdata,u8 *Buf,u8 Pos)
{
	uint32_t temp;
	temp=*(uint32_t *)(&Fdata);
	Buf[Pos] = ((temp>>24)&0x000000FF);
	Buf[Pos+1] = ((temp>>8)&0x0000FF00);
	Buf[Pos+2] = ((temp<<8)&0x00FF0000);
	Buf[Pos+3] = ((temp<<24)&0xFF000000);
}

//Get a float from 4 bytes(modbus) without any conversion (ABCD)
//float modbus_get_float_abcd(const uint16_t *src)
//{
//	float f;
//	uint32_t i;
//}
