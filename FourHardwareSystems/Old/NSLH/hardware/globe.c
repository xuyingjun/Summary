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

uint16_t AdREG[12]={0};                  //modbus 映射寄存器 Ad ch1-ch12
uint16_t DiREG[8]={0};                   //modbus 映射寄存器 Di ch1-ch8
uint16_t DoREG[8]={0};               //modbus 映射寄存器 D0 ch1-ch8
uint16_t DAREG[8]={0};               //modbus 映射寄存器 Da ch1-ch8
int16_t  RTDREG[4]={0};                  //modbus 映射寄存器 RTD ch1-ch4

uint8_t TimerOverFlag=0;
uint8_t BegainRxFlag=0;
uint8_t BusyFlag=0;

u16 SumRec=0;//接收到的校验和
u16 SumCal=0;//计算的校验和

void DealModBus(void)
{
	
	u16 REG0X03[24];//功能吗0x03对应的寄存器地址为0x0001-0x00019  AD12路+RTD4路+DI8路=24
	u16 REG0X06[16];//功能吗0x06对应的寄存器地址为0x0001-0x00010  DA8路+DO8路=16
	u16 DatLength=0;
	u16 StartRegAdd=0;
	u8 i,sendLen;
	if(Uart1_receive_buffer[0]!=modbus_deviceId)  //核对本机地址
	{
		return; //地址错误
	}
	SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);//计算出校验和
	SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];//这里默认接收到的校验和也是低八位在前，高八位在后
	if(SumCal!=SumRec)
	{		
		         //校验和错误
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

	
	for(i=0;i<4;i++)            //RTD寄存器地址对于0x000D~0x0011
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
//	for(i=6;i<8;i++)            //DA寄存器地址对于0x0001~0x0008
//	{
//		REG0X06[i]=DAREG[i];
//	}
//	for(i=0;i<8;i++)            //DO寄存器地址对于0x0009~0x0010
//	{
//		REG0X06[8+i]=DoREG[i];
//	}
	
	StartRegAdd=Uart1_receive_buffer[2]*256+Uart1_receive_buffer[3];//计算其实寄存器地址
	DatLength=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];  //计算寄存器地址长度
	 
	sendLen=0;
	Uart1_Send_buffer[sendLen++]=modbus_deviceId;
	Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[1];
	
	
	switch(Uart1_receive_buffer[1])     //功能码
	{
		case 0x03:
							if((StartRegAdd+DatLength)>16)return;
							Uart1_Send_buffer[sendLen++]=DatLength*2;
							for(i=0;i<DatLength;i++)                     //读取单个或者多个寄存器
							{
								Uart1_Send_buffer[sendLen++]=HoldReg[StartRegAdd+i]>>8;
								Uart1_Send_buffer[sendLen++]=HoldReg[StartRegAdd+i];
							}
			        break;
		case 0x06:						                                //写单个寄存器
							if(StartRegAdd>=16)return;
							HoldReg[StartRegAdd]=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
		case 0x10:
							if(StartRegAdd+DatLength>16) return;    //写多个寄存器
							for(i=0;i<DatLength;i++)
							{
								HoldReg[StartRegAdd+i]=Uart1_receive_buffer[7+2*i]*256+Uart1_receive_buffer[8+2*i];
							}
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
		default:                           //不支持的功能吗
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							Uart1_Send_buffer[1]=0x80;//功能码最高位置1
							Uart1_Send_buffer[2]=0x01; //设置异常码为01-无效功能
							break; 
	}
	SumCal=crc16(&Uart1_Send_buffer[0],sendLen);//计算出校验和
	Uart1_Send_buffer[sendLen++]=SumCal>>8;//crc低字节
  Uart1_Send_buffer[sendLen++]=SumCal&0xff;//crc高字节
	
	RS485MOED_S;                              //485输出
  zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);//发送响应帧
  Delay_ms(30);                              //15ms
	RS485MOED_R;                              //485接收
	
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

/*ModBus 计算CRC码的步骤为：

(1).预置16位寄存器为FFFFH。称此寄存器为CRC寄存器；

(2).把第一个8位数据与CRC寄存器的低位相异或，把结果放于CRC寄存器；

(3).把寄存器的内容右移一位(朝低位)，用0填补最高位，检查最低位；

(4).如果最低位为0：重复第3步(再次移位)如果最低位为1：CRC寄存器与多项式A001（1010 0000 0000 0001）进行异或；

(5).重复步骤3和4，直到右移8次，这样整个8位数据全部进行了处理；

(6).重复步骤2到步骤5，进行下一个8位数据的处理；

(7).最后得到的CRC寄存器即为CRC码。(CRC码 = CRC_L +CRC_H)

*/
/*****************crc校验**********************************************/

//crc生成函数，并将crc存储在预先定于的数组中

//调用方式crc16（指向数据的指针，需要校验的数据长度）
u16 crc16(u8 *ptr,u8 len)
{
	unsigned long wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1
	int i=0,j=0;//定义计数
	for(i=0;i<len;i++)//循环计算每个数据
		{
			wcrc^=*ptr++;//将八位数据与crc寄存器亦或.指针地址增加，指向下个数据
			for(j=0;j<8;j++)//循环计算数据的
				{
					if(wcrc&0X0001)//判断右移出的是不是1，如果是1则与多项式进行异或。
						{
							wcrc=wcrc>>1^0XA001;//先将数据右移一位再与上面的多项式进行异或
						}
					else//如果不是1，则直接移出
						{
							wcrc>>=1;//直接移出
						}
				}
		}
	return wcrc<<8|wcrc>>8;//低八位在前，高八位在后
}

void zh_USART_SendData(u8 ch)
{
   
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//等待上一个字节发送完毕
    USART_SendData(USART1,ch);   
}

//发送len个字节
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
	error=SetValue-RoomTemp;                                  //温度数据都乘以10进行处理
	out=VavleFeedback-(Para[0]*error+Para[1]*error*Para[2]);  //PID parameters should be adjusted
	
	//限幅
	if(out<0.0)
		out=0.0;
	else if(out>=10.0)
		out=10.0;
	
	return out;
}
