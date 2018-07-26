#include "modbus.h"
//#include "winsock2.h"

u16 SumRec=0;//接收到的校验和
u16 SumCal=0;//计算的校验和

void DealModBus(void)
{
	u8 REG0X03[100];    //功能吗0x03对应的寄存器地址为0x0001-0x00064  
	u16 REG0X06[100];    //功能吗0x06对应的寄存器地址为0x0001-0x00064  
	
	u16 DatLength=0;     //数据长度
	u16 StartRegAdd=0;   //起始地址
	
	u8 i,sendLen;        //相关变量
//	float temp=0.0;
	if(Uart1_receive_buffer[0]!=modbus_deviceId)  //核对本机地址
	{
		return; //地址错误
	}
	
	SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);//计算出校验和
	SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];  //这里默认接收到的校验和也是低八位在前，高八位在后
	if(SumCal!=SumRec)           //校验和不一致
	{         
		return ;                   //校验和错误
	}
	
	//将数据存到读寄存器中，地址为0x01-0x64
//	temp=advalue[1]*1000.0;
//	REG0X03[0]=*(uint32_t *)(&temperature)/65536;
//	REG0X03[1]=*(uint32_t *)(&temperature)%65536;
	//FloatPut(temperature,REG0X03,0);
	//FloatGet(temperature,REG0X03,4);
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
		SumCal=crc16(&Uart1_Send_buffer[0],sendLen);            //计算出校验和
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal>>8);      //crc高字节
		Uart1_Send_buffer[sendLen++]=(uint8_t)(SumCal&0xff);    //crc低字节
	}
	
	RS485MOED_S;                                            //485输出
  zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);     //发送响应帧
  delay_nms(2);
	RS485MOED_R;                                            //485接收
	
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
	unsigned long wcrc=0XFFFF;    //预置16位crc寄存器，初值全部为1
	int i=0,j=0;                  //定义计数
	for(i=0;i<len;i++)            //循环计算每个数据
		{
			wcrc^=*ptr++;             //将八位数据与crc寄存器亦或.指针地址增加，指向下个数据
			for(j=0;j<8;j++)          //循环计算数据的
				{
					if(wcrc&0X0001)       //判断右移出的是不是1，如果是1则与多项式进行异或。
						{
							wcrc=wcrc>>1^0XA001;  //先将数据右移一位再与上面的多项式进行异或
						}
					else                      //如果不是1，则直接移出
						{
							wcrc>>=1;             //直接移出
						}
				}
		}
	return wcrc<<8|wcrc>>8;           //低八位在前，高八位在后
}

//将float类型的数据存放在uchar类型的数组Buf中，存储在Buf的第Pos个字节开始的连续四个地址
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
