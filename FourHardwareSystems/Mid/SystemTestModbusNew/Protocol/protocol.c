#include "protocol.h"

////////////////////////////////////////////////////
///<功能描述>解析接收到的数据的包头
///<参数> 指针，指向接收缓存寄存器首地址
///<说明> 包头由10个字节构成
///<返回值>null
///////////////////////////////////////////////////
/************************************************************************
1-4：地址位；分为控制板地址和端口地址，1-控制表高字节，2-控制表低字节，3-4：端口地址为0
5：模式位; 固定位0x02,与控制板有关
6：功能码; 读端口(0x55),写端口(0x66),读PID(0x77),写PID(0x88)
7：校验位; 数据段求和，然后对总和取100的余数
8-9：数据长度; 2个字节表示数据字节的长度，高位在前，低位在后
10：保留位; 固定数值为0
************************************************************************/
int PacketHeaderAnalysis(char *data)
{
	int length;
  length = data[7]*100 + data[8];       //求出数据段长度
	//判断模式位
	if(data[4] == 0x01)                   //模式为0x01
		{
			MCUaddress = data[0]*100 + data[1];  //计算控制板地址
			StageSel =  CONFIG;                  //状态选择
			ModeSel = (MODESEL)data[5];          //任务选择
			LineSel = (LINESEL)data[9];          //online---offline
		}
  else if(data[4] == 0x02)              //模式为0x02
		{
			StageSel =  RUNTIME;
			TaskSel  =  (TASKSEL)data[5];
		}
  
	return length;                        //返回数据段字节长度
}

void PacketHeaderState(char * data)
{
	if(data[4] == 0x01)
		{
			MCUaddress = data[0]*100 + data[1];
	    StageSel =  CONFIG;
	    LineSel = (LINESEL)data[9];
    }
  else if(data[4] == 0x02)
		{
			StageSel =  RUNTIME;
	    TaskSel  =  (TASKSEL)data[5];
    }  
}

//产生发送数据，包头，数据包，包尾
//length:数据段长度,dataBuffer：为数据段数据， sentBuffer:发送缓存，包括包头，数据段和包尾
void SentDataGeneration(int length,char* dataBuffer,char* sentBuffer)
{
	int i = 0,j;

	//产生数据发送头文件
  sentBuffer[i++] = MCUaddress/100;	  
  sentBuffer[i++] = MCUaddress%100;       //Address
  sentBuffer[i++] = 0x00;
  sentBuffer[i++] = 0x00;
  sentBuffer[i++] = (int)StageSel;	      //StageSel
  if(StageSel == CONFIG)
		sentBuffer[i++] = (int)ModeSel;
	else								                    //FuncSel
		sentBuffer[i++] = (int)TaskSel;
  sentBuffer[i++] = checkSentData(length,dataBuffer); //CheckCode
  sentBuffer[i++] = length/100;                       //Length
  sentBuffer[i++] = length%100; 
  sentBuffer[i++] = 0;	
	
	//打包数据段
  for(j=0;j<length;j++)
	{
		sentBuffer[i++] = dataBuffer[j];
  }
   
	sentBuffer[i++] = 0xff;
  sentBuffer[i++] = 0xff;
}

////////////////////////////////////////////////////
///<功能描述>校验接收到的数据段数据是否正确
///<参数1> 指针，指向接收缓存寄存器首地址(包头首字)
///<返回值> 0 -- 数据传输失真，1--数据传输正确
///////////////////////////////////////////////////
int CheckReceivedData(char* receivedBuffer)
{
	int i; 
  long result = 0;		                   //累加和
  char calculatedCheckCode;  	           //产生的校验码
  char receivedCheckCode;                //接收的校验码
  int dataLength;                        //数据段数据的长度
  unsigned int address;                  //地址段校验
	
	//判断模式位，固定位0x02
	if(receivedBuffer[4] == 0x02)
		{
		//判断控制器地址
			address = receivedBuffer[0]*100 + receivedBuffer[1];
			if( MCUaddress != address)  
				return 0;
		}
	
	//存储接收到的校验码
	receivedCheckCode = receivedBuffer[6];
	//计算接收数据长度，数据段长度
  dataLength = receivedBuffer[7]*100 + receivedBuffer[8];
	//计算接收数据校验和
  for(i=0;i<dataLength;i++)
  {
		result += receivedBuffer[i+LENGTH];
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

//得到数据整数部分高字节位
char getIntHighByte(float data)
{
	char highByte = 0;
  int temp = (int)data;
	if(temp<0)
		{
			temp = temp * (-1);
			highByte = temp/100;
			highByte = highByte | 0x80;
		}
	else
		highByte = temp/100;
	
	return highByte;
}

//得到数据整数部分低字节位
char getIntLowByte(float data)
{
	char lowByte = 0;
  int temp = (int)data;
  if(temp<0)
		temp = temp * (-1);
	lowByte = temp % 100;
  return lowByte;
}

//得到数据小数部分高字节数据
char getDecHighByte(float data)
{
	char decByte = 0;
	if(data<0)
		data = data * (-1);
	decByte = ((int)(data*10000) % 10000) / 100;
	return decByte;
}

//得到数据小数部分低字节数据
char getDecLowByte(float data)
{
    char decByte = 0;
	if(data<0)
	   data = data * (-1);
	decByte = (int)(data*10000) % 100;
	return decByte;
}
