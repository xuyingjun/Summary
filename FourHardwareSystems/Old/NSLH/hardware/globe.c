#include "Globe.h"

extern float dataTemp[12];
extern float Para[3];
extern float AI_Input[12];

uint8_t Uart1_SCnt=0;
uint8_t Uart1_Send_buffer[150]={0};
uint8_t Uart1_receive_buffer[150]={0};
uint8_t Uart1_RCnt=0;
uint8_t Uart1_SendBuffSize=0;

uint8_t Uart2_SCnt=0;
uint8_t Uart2_Send_buffer[150]={0};
uint8_t Uart2_receive_buffer[150]={0};
uint8_t Uart2_RCnt=0;
uint8_t Uart2_SendBuffSize=0;

uint8_t Uart3_SCnt=0;
uint8_t Uart3_Send_buffer[150]={0};
uint8_t Uart3_receive_buffer[150]={0};
uint8_t Uart3_RCnt=0;
uint8_t Uart3_SendBuffSize=0;

uint8_t Uart4851_TestFlag=0;
uint8_t Uart4852_TestFlag=0;
uint8_t Wifi_TestFlag=0;

uint16_t AdREG[12]={0};                  //modbus ӳ��Ĵ��� Ad ch1-ch12
uint16_t DiREG[8]={0};                   //modbus ӳ��Ĵ��� Di ch1-ch8
uint16_t DoREG[8]={0};               //modbus ӳ��Ĵ��� D0 ch1-ch8
uint16_t DAREG[8]={0};               //modbus ӳ��Ĵ��� Da ch1-ch8
int16_t  RTDREG[4]={0};                  //modbus ӳ��Ĵ��� RTD ch1-ch4

uint8_t TimerOverFlag=0;
uint8_t BegainRxFlag=0;
uint8_t BusyFlag=0;

u16 SumRec=0;//���յ���У���
u16 SumCal=0;//�����У���

void DealModBus(void)
{
	
	u16 REG0X03[24];//������0x03��Ӧ�ļĴ�����ַΪ0x0001-0x00019  AD12·+RTD4·+DI8·=24
	u16 REG0X06[16];//������0x06��Ӧ�ļĴ�����ַΪ0x0001-0x00010  DA8·+DO8·=16
	u16 DatLength=0;
	u16 StartRegAdd=0;
	u8 i,sendLen;
	if(Uart1_receive_buffer[0]!=modbus_deviceId)  //�˶Ա�����ַ
	{
		return; //��ַ����
	}
	SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);//�����У���
	SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];//����Ĭ�Ͻ��յ���У���Ҳ�ǵͰ�λ��ǰ���߰�λ�ں�
	if(SumCal!=SumRec)
	{		
		         //У��ʹ���
		return ;
	}

	HoldReg[0]=(RoomTemp*10);                 //Room temperature
	HoldReg[1]=(RoomTempSv);               //Room temperature setvalue
	HoldReg[2]=(RoomHum*10);                  //Room Humidity
	HoldReg[3]=60;                       //Room Humidity set value
	HoldReg[4]=(RTDtemperature[2]*10);        //chiller water outlet temperature
	HoldReg[5]=CO2;                      //CO2 Value
	HoldReg[6]=CO2Sv;                    //CO2 setpoint value
	HoldReg[7]=0;                        //Motion sensor status
	HoldReg[8]=0;                        //Euqipment communication software
	HoldReg[9]=(dataTemp[11]*10);      //valve feedback value
	HoldReg[10]=0;
	HoldReg[11]=0;
//	REG0X03[0]=(RoomTemp*10);                 //Room temperature
//	REG0X03[1]=(RoomTempSv*10);               //Room temperature setvalue
//	REG0X03[2]=(RoomHum*10);                  //Room Humidity
//	REG0X03[3]=60;                       //Room Humidity set value
//	REG0X03[4]=(RTDtemperature[2]*10);        //chiller water outlet temperature
//	REG0X03[5]=CO2;                      //CO2 Value
//	REG0X03[6]=CO2Sv;                    //CO2 setpoint value
//	REG0X03[7]=0;                        //Motion sensor status
//	REG0X03[8]=0;                        //Euqipment communication software
//	REG0X03[9]=(dataTemp[11]*10);      //valve feedback value
//	REG0X03[10]=0;
//	REG0X03[11]=0;

	
	for(i=0;i<4;i++)            //RTD�Ĵ�����ַ����0x000D~0x0011
	{
		HoldReg[12+i]=0;
	}
		
//	REG0X06[0]=PDVComm;         //system start command
//	REG0X06[1]=RoomTempSv;      //Room Temperature Setvalue
//	REG0X06[2]=RoomHumSv;       //Room Humidity SetValue
//	REG0X06[3]=InletTemp;       //Inlet water temperature
//	REG0X06[4]=ScheduleComm;    //system booking command
//	REG0X06[5]=IBSCommand;       //Internet Booking system Command
//	
//	for(i=6;i<8;i++)            //DA�Ĵ�����ַ����0x0001~0x0008
//	{
//		REG0X06[i]=DAREG[i];
//	}
//	for(i=0;i<8;i++)            //DO�Ĵ�����ַ����0x0009~0x0010
//	{
//		REG0X06[8+i]=DoREG[i];
//	}
	
	StartRegAdd=Uart1_receive_buffer[2]*256+Uart1_receive_buffer[3];//������ʵ�Ĵ�����ַ
	DatLength=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];  //����Ĵ�����ַ����
	 
	sendLen=0;
	Uart1_Send_buffer[sendLen++]=modbus_deviceId;
	Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[1];
	
	
	switch(Uart1_receive_buffer[1])     //������
	{
		case 0x03:
							if((StartRegAdd+DatLength)>16)return;
							Uart1_Send_buffer[sendLen++]=DatLength*2;
							for(i=0;i<DatLength;i++)                     //��ȡ�������߶���Ĵ���
							{
								Uart1_Send_buffer[sendLen++]=HoldReg[StartRegAdd+i]>>8;
								Uart1_Send_buffer[sendLen++]=HoldReg[StartRegAdd+i];
							}
			        break;
		case 0x06:						                                //д�����Ĵ���
							if(StartRegAdd>=16)return;
							HoldReg[StartRegAdd]=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
		case 0x10:
							if(StartRegAdd+DatLength>16) return;    //д����Ĵ���
							for(i=0;i<DatLength;i++)
							{
								HoldReg[StartRegAdd+i]=Uart1_receive_buffer[7+2*i]*256+Uart1_receive_buffer[8+2*i];
							}
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
		default:                           //��֧�ֵĹ�����
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							Uart1_Send_buffer[1]=0x80;//���������λ��1
							Uart1_Send_buffer[2]=0x01; //�����쳣��Ϊ01-��Ч����
							break; 
	}
	SumCal=crc16(&Uart1_Send_buffer[0],sendLen);//�����У���
	Uart1_Send_buffer[sendLen++]=SumCal>>8;//crc���ֽ�
  Uart1_Send_buffer[sendLen++]=SumCal&0xff;//crc���ֽ�
	
	RS485MOED_S;                              //485���
  zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);//������Ӧ֡
  Delay_ms(30);                              //15ms
	RS485MOED_R;                              //485����
	
	PDVComm=HoldReg[0];
	if(PDVComm!=0)
		PDVComm=1;
	else
		PDVComm=0;
	
	RoomTempSv=HoldReg[1];
	RoomHumSv=HoldReg[2];
	InletTemp=HoldReg[3];
	
	ScheduleComm=HoldReg[4];
	if(ScheduleComm!=0)
		ScheduleComm=1;
	else
		ScheduleComm=0;
	
	IBSCommand=HoldReg[5];
	if(IBSCommand!=0)
		IBSCommand=1;
	else
		IBSCommand=0;	
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

float ValveControl(float SetValue)
{
	float out=10.0;
	VavleFeedback=AI_Input[11];
	//VavleFeedback=10.0;
	if(VavleFeedback>10.0)
		VavleFeedback=10.0;
	error=SetValue-RoomTemp;                                  //�¶����ݶ�����10���д���
	out=VavleFeedback-(Para[0]*error+Para[1]*error*Para[2]);  //PID parameters should be adjusted
	
	//�޷�
	if(out<0.0)
		out=0.0;
	else if(out>=10.0)
		out=10.0;
	
	return out;
}
