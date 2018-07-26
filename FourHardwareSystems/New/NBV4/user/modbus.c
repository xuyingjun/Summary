#include "modbus.h"
#include "Globle.h"

u16 SumRec=0;//接收到的校验和
u16 SumCal=0;//计算的校验和
u16 SUMCAL=0; //查表法

void DealModBus(void)
{
	u16 REG0X03[100];    //功能吗0x03对应的寄存器地址为0x0001-0x00064  
	u16 REG0X06[100];    //功能吗0x06对应的寄存器地址为0x0001-0x00064  
	
	u16 DatLength=0;     //数据长度
	u16 StartRegAdd=0;   //起始地址
	
	u8 i,sendLen;        //相关变量
	
	if(Uart1_receive_buffer[0]!=modbus_deviceId)  //核对本机地址
	{
		return; //地址错误
	}
	SUMCAL=CRC16(&Uart1_receive_buffer[0],Uart1_RCnt-2);
	SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);//计算出校验和
	SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];  //这里默认接收到的校验和也是低八位在前，高八位在后
	if(SUMCAL!=SumRec)           //校验和不一致
	{         
		return ;                   //校验和错误
	}
	
	//将数据存到读寄存器中，地址为0x01-0x64
	for(i=0;i<100;i++)
	{
		REG0X03[i]=ReadReg[i];
	}
	
	//将数据存到写寄存器中，地址为0x01-0x64
	for(i=0;i<100;i++)
	{
		REG0X06[i]=WriteReg[i];
	}
	
	//0---Device Id 1---funciton code  2,3---寄存器开始地址   4,5---寄存器地址长度
	StartRegAdd=Uart1_receive_buffer[2]*256+Uart1_receive_buffer[3];   //计算其实寄存器地址
	DatLength=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];     //计算寄存器地址长度
	
	//判断初始地址是否合理，合理的话返回相应的请求应答消息
	if((StartRegAdd+DatLength)<0x64)
	{
		sendLen=0;
		Uart1_Send_buffer[sendLen++]=modbus_deviceId;                  //设备ID
		Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[1];          //对应功能码
		
		switch(Uart1_receive_buffer[1])     //功能码，根据不同的功能码，返回对应的应答消息
		{
			case 0x03:
							if((StartRegAdd+DatLength)>100)return;
							Uart1_Send_buffer[sendLen++]=DatLength*2;    //每个寄存器值存储有2个字节，所以数据长度为2*N(寄存器数量)
							for(i=0;i<DatLength;i++)                     //读取单个或者多个寄存器
							{
								Uart1_Send_buffer[sendLen++] = (uint8_t)REG0X03[StartRegAdd+i]>>8;        //赋值高字节
								Uart1_Send_buffer[sendLen++]=(uint8_t)(REG0X03[StartRegAdd+i]&0xFF);      //赋值低字节
							}
			        break;
			case 0x06:						                                //写单个寄存器
							if(StartRegAdd>=100)return;                 //写寄存器的地址大于100，则返回
							
							REG0X06[StartRegAdd]=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5]; //将需要些的数据存储到指定的地址里
							
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];  //返回相应的寄存器地址和数据
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
			case 0x10:
							if(StartRegAdd+DatLength>=100)return;                       //写多个寄存器超过地址范围
							for(i=0;i<DatLength;i++)
							{
								REG0X06[StartRegAdd+i]=Uart1_receive_buffer[7+i*2]*256+Uart1_receive_buffer[8+i*2];
							}
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];  //返回相应的寄存器地址和数据
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							break;
							
			default:                           //不支持的功能吗
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
							Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
							Uart1_Send_buffer[1]=0x80;                             //功能码最高位置1
							Uart1_Send_buffer[2]=0x01;                             //设置异常码为01-无效功能
							break; 
		}
	
		//发送数据校验和
		SUMCAL=CRC16(&Uart1_Send_buffer[0],sendLen);
		SumCal=crc16(&Uart1_Send_buffer[0],sendLen);            //计算出校验和
//		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal>>8);      //crc高字节
//		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal&0xff);    //crc低字节
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SUMCAL>>8);      //crc高字节
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SUMCAL&0xff);    //crc低字节
	}
	
	RS485MOED_S;  //DE1=0                                          //485输出
  zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);     //发送响应帧
  delay_ms(100);
	RS485MOED_R;  //DE1=1;                                          //485接收
	
	for(i=0;i<100;i++)
	{
		WriteReg[i]=REG0X06[i];	
	}
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

int CheckTSReceivedData(char* receivedBuffer)
{
	int i; 
  long result = 0;		                   //累加和
  char calculatedCheckCode;  	           //产生的校验码
  char receivedCheckCode;                //接收的校验码

	receivedCheckCode=receivedBuffer[21];
	
	for(i=0;i<18;i++)
	{
		result += receivedBuffer[i+TSLENGTH];
  }
	
	calculatedCheckCode = result % 100;
	
	//判断两个校验和是否正确
	if(calculatedCheckCode == receivedCheckCode)
		return 1;
  else
    return 0;
}

////////////////////////////////////////////////////
///<功能描述>校验待发送的数据
///<参数1> 指针，指向发送缓存寄存器的数据段首地址
///<返回值> 返回校验码
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
  int isTrue = 0; 	//校验是否正确 1--true
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
