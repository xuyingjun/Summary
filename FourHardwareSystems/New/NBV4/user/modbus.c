#include "modbus.h"
#include "Globle.h"

u16 SumRec=0;//���յ���У���
u16 SumCal=0;//�����У���
u16 SUMCAL=0; //���

void DealModBus(void)
{
	u16 REG0X03[100];    //������0x03��Ӧ�ļĴ�����ַΪ0x0001-0x00064  
	u16 REG0X06[100];    //������0x06��Ӧ�ļĴ�����ַΪ0x0001-0x00064  
	
	u16 DatLength=0;     //���ݳ���
	u16 StartRegAdd=0;   //��ʼ��ַ
	
	u8 i,sendLen;        //��ر���
	
	if(Uart1_receive_buffer[0]!=modbus_deviceId)  //�˶Ա�����ַ
	{
		return; //��ַ����
	}
	SUMCAL=CRC16(&Uart1_receive_buffer[0],Uart1_RCnt-2);
	SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);//�����У���
	SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];  //����Ĭ�Ͻ��յ���У���Ҳ�ǵͰ�λ��ǰ���߰�λ�ں�
	if(SUMCAL!=SumRec)           //У��Ͳ�һ��
	{         
		return ;                   //У��ʹ���
	}
	
	//�����ݴ浽���Ĵ����У���ַΪ0x01-0x64
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
		SUMCAL=CRC16(&Uart1_Send_buffer[0],sendLen);
		SumCal=crc16(&Uart1_Send_buffer[0],sendLen);            //�����У���
//		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal>>8);      //crc���ֽ�
//		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal&0xff);    //crc���ֽ�
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SUMCAL>>8);      //crc���ֽ�
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SUMCAL&0xff);    //crc���ֽ�
	}
	
	RS485MOED_S;  //DE1=0                                          //485���
  zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);     //������Ӧ֡
  delay_ms(100);
	RS485MOED_R;  //DE1=1;                                          //485����
	
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
	unsigned long wcrc=0XFFFF;//Ԥ��16λcrc�Ĵ�������ֵȫ��Ϊ1
	int i=0,j=0;//�������
	for(i=0;i<len;i++)//ѭ������ÿ������
		{
			wcrc^=*ptr++;//����λ������crc�Ĵ������.ָ���ַ���ӣ�ָ���¸�����
			for(j=0;j<8;j++)//ѭ���������ݵ�
				{
					if(wcrc&0X0001)//�ж����Ƴ����ǲ���1�������1�������ʽ�������
						{
							wcrc=wcrc>>1^0XA001;//�Ƚ���������һλ��������Ķ���ʽ�������
						}
					else//�������1����ֱ���Ƴ�
						{
							wcrc>>=1;//ֱ���Ƴ�
						}
				}
		}
	return wcrc<<8|wcrc>>8;//�Ͱ�λ��ǰ���߰�λ�ں�
}

void zh_USART_SendData(u8 ch)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ���һ���ֽڷ������
    USART_SendData(USART1,ch);   
}

//����len���ֽ�
void zh_USART_SendString(u8* s,u8 len)
{
	  
    int i;
    for(i=0;i<len;i++)
    {
        zh_USART_SendData(s[i]);
    }
}

int CheckTSReceivedData(char* receivedBuffer)
{
	int i; 
  long result = 0;		                   //�ۼӺ�
  char calculatedCheckCode;  	           //������У����
  char receivedCheckCode;                //���յ�У����

	receivedCheckCode=receivedBuffer[21];
	
	for(i=0;i<18;i++)
	{
		result += receivedBuffer[i+TSLENGTH];
  }
	
	calculatedCheckCode = result % 100;
	
	//�ж�����У����Ƿ���ȷ
	if(calculatedCheckCode == receivedCheckCode)
		return 1;
  else
    return 0;
}

////////////////////////////////////////////////////
///<��������>У������͵�����
///<����1> ָ�룬ָ���ͻ���Ĵ��������ݶ��׵�ַ
///<����ֵ> ����У����
///////////////////////////////////////////////////
char checkSentData(int length,char* dataBuffer)
{
	int i; 
  long result = 0;
  char checkCode;
	
	for(i=0;i<length;i++)
  {
		result += dataBuffer[i];
  }
	checkCode = result % 100;
	return checkCode;
}

int TSPacketAnalysis(char *data)
{
  int isTrue = 0; 	//У���Ƿ���ȷ 1--true
  isTrue    = CheckTSReceivedData(data);    
  if(isTrue)
		{   
			if((data[0]==0x55)||(data[1]==0x66))
				{
					TSTaskSel  =  (TSTASKSEL)data[2];
				}
			return 1;
		}   
	return 0;   
}

void TSSentDataGeneration(int length,char* dataBuffer,char* sentBuffer)
{
	int i = 0,j;
	sentBuffer[i++]=0x55;
	sentBuffer[i++]=0x66;
	sentBuffer[i++]=(int)TSTaskSel;
	
	for(j=0;j<length;j++)
	{
		sentBuffer[i++] = dataBuffer[j];
  }
	
	sentBuffer[i++] = checkSentData(length,dataBuffer);
	sentBuffer[i++] = 0x66;
	sentBuffer[i++] = 0x55;
}

void TSDataProcess(void)
{
	switch(TSTaskSel)
	{
		case TSNON: break;
		case READDATA: GetTouchScreenValue();break;
		case SETDATA: break;
		case START: TSControl=1; break;
		case TSSTOP: TSControl=0; break;
		default: break;
	}
	TSTaskSel = TSNON;
}

void GetTouchScreenValue()
{
	if(TSTaskSel==READDATA)
		{
			CO2=TSReceivedBuffer[3]*100+TSReceivedBuffer[4];
			CO2Sv=TSReceivedBuffer[5]*100+TSReceivedBuffer[6];
			RoomTemp=(TSReceivedBuffer[7]*100+TSReceivedBuffer[8])/10.0;
			RoomTempSv=(TSReceivedBuffer[9]*100+TSReceivedBuffer[10])/10.0;
			RoomHum=(TSReceivedBuffer[11]*100+TSReceivedBuffer[12])/10.0;
		}
}


u16 CRC16(u8 *Pushdata,u8 length)
{
	u8 uchCRCHi=0xff;
	u8 uchCRCLo=0xff;
	u8 uIndex;
	while(length--)
	{
		uIndex=uchCRCHi^*Pushdata++;
		uchCRCHi=uchCRCLo^aucCRCHi[uIndex];
		uchCRCLo=aucCRCLo[uIndex];
	}
	return (uchCRCHi<<8|uchCRCLo);	
}
