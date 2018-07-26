#include "touchscreen.h"
#include "runtimedata.h"
#include "protocol.h"

void TSDataProcess(void)
{
	switch(TSTaskSel)
	{
		case TSNON: break;
		case READDATA: TSreadPorts(TSReadPortsBuffer);GetTouchScreenValue();break;
		case SETDATA: break;
		case START: TSControl=1; break;
		case TSSTOP: TSControl=0; break;
		default: break;
	}
	TSTaskSel = TSNON;
}

void TSreadPorts(char* readPortsBuffer)
{
	int i = 0;
  int j;
	
	APortCIn(ADC_data);
	for(j=0;j<5;j++)
	{
		ADC_value[j] = getAIPhysicalValue(ADC_data[j],AIParams[j]);
		readPortsBuffer[i++]=getIntHighByte(ADC_value[j]);
		readPortsBuffer[i++]=getIntLowByte(ADC_value[j]);
		readPortsBuffer[i++]=getDecHighByte(ADC_value[j]);
		readPortsBuffer[i++]=getDecLowByte(ADC_value[j]);
	}	
	RTD_allchannels(RTD_value);
	for(j=0;j<3;j++)
	{
		readPortsBuffer[i++]=getIntHighByte(RTD_value[j]);
		readPortsBuffer[i++]=getIntLowByte(RTD_value[j]);
		readPortsBuffer[i++]=getDecHighByte(RTD_value[j]);
		readPortsBuffer[i++]=getDecLowByte(RTD_value[j]);
	}	
	
	DPortCIn(DI_data);	
	readPortsBuffer[i++]=DI_data[0];
	readPortsBuffer[i++]=DI_data[1];
}


void TSSentDataGeneration(int length,char* dataBuffer,char* sentBuffer)
{
	int i = 0,j;
	sentBuffer[i++]=0x55;
	sentBuffer[i++]=0x66;
	sentBuffer[i++]=(int)TSTaskSel;
	sentBuffer[i++]=(u8)(RoomTempSv/256);
	sentBuffer[i++]=(u8)(RoomTempSv%256);

	for(j=0;j<length;j++)
	{
		sentBuffer[i++] = dataBuffer[j];
  }
	
	sentBuffer[i++] = checkSentData(length,dataBuffer);
	sentBuffer[i++] = 0x66;
	sentBuffer[i++] = 0x55;
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

void GetTouchScreenValue()
{
	if(TSTaskSel==READDATA)
		{
			
			CO2=TSReceivedBuffer[3]*100+TSReceivedBuffer[4];
			CO2Sv=TSReceivedBuffer[5]*100+TSReceivedBuffer[6];
			RoomTemp=(TSReceivedBuffer[7]*100+TSReceivedBuffer[8]);
			//RoomTempSv=(TSReceivedBuffer[9]*100+TSReceivedBuffer[10]);
			RoomHum=(TSReceivedBuffer[11]*100+TSReceivedBuffer[12]);
		}
}
