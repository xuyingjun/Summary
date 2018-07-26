#include "runtimedata.h"

//���ݴ���
void DataProcess(void)
{
	switch(TaskSel)
  {
		case NON:          break;
	  case WRITEPORTS:	 writePorts(WritePortsBuffer);break;
	  case READPORTS:    readPorts(ReadPortsBuffer);break;	 
	  case WRITEPID:     writePID(WritePIDBuffer); break;
	  case READPID:      readPID(ReadPIDBuffer);break;
  }
	TaskSel = NON;
}


/*********************************************************************************************************
* Functoin name:       void ReadPortsData(void)
* Descriptions:        �������ȡ���ж˿����ݣ�����䷢�ͻ�������send_buffer
*                      ��ȡ˳��AI,RI,AO,DI,DO,RGE  ��������Ч����165
* input paraments:     ��
* output paraments:    ��    
* Returned values:     ��
*********************************************************************************************************/
void readPorts(char* readPortsBuffer)
{
	int i = 0;
  int j;
  char temp = 0;  
	
	//��ȡAI�˿�����  AI1-AI12  12*4=48
  //APortCIn(ADC_data);
  for(j=0;j<12;j++)		
  {
		ADC_value[j] = getAIPhysicalValue(ADC_data[j],AIParams[j]);
    readPortsBuffer[i++] = getIntHighByte(ADC_value[j]);	           //intH
	  readPortsBuffer[i++] = getIntLowByte(ADC_value[j]);	             //intL
	  readPortsBuffer[i++] = getDecHighByte(ADC_value[j]);             //decH
	  readPortsBuffer[i++] = getDecLowByte(ADC_value[j]);	             //decL
  }
	
  //��ȡRI�˿����� RI1-RI4,RED���ݣ��¶ȴ���������  4*4=16
  //RTD_allchannels(RTD_value);
  for(j=0;j<4;j++)	       
  {
		readPortsBuffer[i++] = getIntHighByte(RTD_value[j]);	           //intH
	  readPortsBuffer[i++] = getIntLowByte(RTD_value[j]);	             //intL
	  readPortsBuffer[i++] = getDecHighByte(RTD_value[j]);             //decH
	  readPortsBuffer[i++] = getDecLowByte(RTD_value[j]);	             //decL
  }
	
  //��ȡAOAMλ   1
  for(j=0;j<8;j++)	      
  {
		if(AOAM[j] == CLOSELOOP)
			temp |= (0x01<<j);
  } 
  readPortsBuffer[i++] = temp;
  temp = 0;
   
	//��ȡAO�˿�ֵ AO1-AO8            8*4=32
	//APortCOut(DAC_data);
  for(j=0;j<8;j++)		
  {
		DAC_value[j] = getAOPhysicalValue(DAC_data[j],AOParams[j]);
		readPortsBuffer[i++] = getIntHighByte(DAC_value[j]);	    //intH
	  readPortsBuffer[i++] = getIntLowByte(DAC_value[j]);	   //intL
	  readPortsBuffer[i++] = getDecHighByte(DAC_value[j]);     //decH
	  readPortsBuffer[i++] = getDecLowByte(DAC_value[j]);	   //decL
  }
   
	//��ȡDI�˿�ֵ                  1
  //DPortCIn(DI_data);
  for(j=0;j<8;j++)	  
  {
		if(DI_data[j]) 
			temp |=(0x01<<j);
  }
  readPortsBuffer[i++] = temp;
  temp = 0;
	
  //��ȡDO�˿�ֵ                 1
	//DPortCOut(DO_data);
  for(j=0;j<8;j++)	 
  {
		if(DO_data[j]) 
			temp |=(1<<j);
  }
  readPortsBuffer[i++] = temp;
  temp = 0;

	//��ȡREGAM���ȷŸ�λ���ٷŵ�λ            2
	for(j=0;j<8;j++)	      //REGAM9-16
  {
		if(REGAM[j+8] == CLOSELOOP)
			temp |= (0x01<<j);
  }
  readPortsBuffer[i++] = temp;
  temp = 0;
	
  for(j=0;j<8;j++)	      //REGAM1-8
  {
		if(REGAM[j] == CLOSELOOP)
			temp |= (0x01<<j);
  }  
  readPortsBuffer[i++] = temp;
  temp = 0;
	
	Registers[0]=CO2;
	Registers[1]=CO2Sv;
	Registers[2]=RoomTemp;
	Registers[3]=RoomTempSv;
	Registers[4]=RoomHum;
	
	
	//��ȡREG REG1-REG16             16*4=64
  for(j=0;j<16;j++)		
  {
		readPortsBuffer[i++] = getIntHighByte(Registers[j]);	    //intH
	  readPortsBuffer[i++] = getIntLowByte(Registers[j]);	      //intL
	  readPortsBuffer[i++] = getDecHighByte(Registers[j]);      //decH
	  readPortsBuffer[i++] = getDecLowByte(Registers[j]);	      //decL
  }
}

/*********************************************************************************************************
* Functoin name:       void WritePortsData(void)
* Descriptions:        �������ö˿����� ��������Ч����38λ��12+26��
*                      ����˳��AO,DO,Reg
* input paraments:     ��
* output paraments:    ��    
* Returned values:     ��
*********************************************************************************************************/
void writePorts(char* writePortsBuffer)
{
	int j;
	int i = 0;
  char temp,temp1;
  char intHighByte,intLowByte,decHighByte,decLowByte,sign;
  float Tempdata,value;
	
	temp = writePortsBuffer[i++];  //AOAM
  for(j=0;j<8;j++)
  {
		if((temp>>j)&0x01)	
			AOAM[j] = CLOSELOOP;
		else 
			AOAM[j] = OPENLOOP;
  }
	
	temp = writePortsBuffer[i++];  //AO_HANDLER��DAQ�ж������Ƿ��·�����1���·��������������
  for(j=0;j<8;j++)	              //AO
  {
		intHighByte = writePortsBuffer[i++];
	  intLowByte  = writePortsBuffer[i++];
	  decHighByte = writePortsBuffer[i++];
	  decLowByte  = writePortsBuffer[i++];  
    if((temp>>j)&0x01)	
    {
			//�ж�������
			sign = (intHighByte & 0x80)>>7;
			value = (intHighByte&0x7f)*100 + intLowByte + decHighByte/100.0 + decLowByte/10000.0;	
			//����
			if(sign) 
				value = value * (-1);
			DAC_value[j] = value;
			Tempdata = (value - AOParams[j].b)/AOParams[j].a;
     	//APortOut(j,data);
    }
  }

  temp  = writePortsBuffer[i++];  //DO_HANDLER
  temp1 = writePortsBuffer[i++];  //DO
  for(j=0;j<8;j++)	  //DO
  {
		if((temp>>j) & 0x01)
	  {
			if((temp1>>j) & 0x01)
			{
				//DPortOut(j,1);
				DO_data[j] = 1;
		  }
		  else 
		  {
				//DPortOut(j,0);
				DO_data[j] = 0;
			}
	  }
  } 

  temp = writePortsBuffer[i++];  //REGAM9--16
  for(j=0;j<8;j++)
  {
		if((temp>>j)&0x01)	
			REGAM[j+8] = CLOSELOOP;
		else 
			REGAM[j+8] = OPENLOOP;
  }
  temp = writePortsBuffer[i++];  //REGAM1--8
  for(j=0;j<8;j++)
  {
		if((temp>>j)&0x01)
			REGAM[j] = CLOSELOOP;
		else 
			REGAM[j] = OPENLOOP;
  }
   
  temp1 = writePortsBuffer[i++];  	//REG_HANDLER9--16
  temp = writePortsBuffer[i++];  	  //REG_HANDLER1--8
  for(j=0;j<8;j++)	               //REG1--8
  {
		intHighByte = writePortsBuffer[i++];
	  intLowByte  = writePortsBuffer[i++];
	  decHighByte = writePortsBuffer[i++];
	  decLowByte  = writePortsBuffer[i++];  
    if((temp>>j)&0x01)	
    {
			sign = (intHighByte & 0x80)>>7;
	    value = (intHighByte&0x7f)*100 + intLowByte + decHighByte/100.0 + decLowByte/10000.0;	
			if(sign) 
				value = value * -1;	
			Registers[j] = value;
    }
  }
	
	for(j=0;j<8;j++)	              //REG9--16
  {
		intHighByte = writePortsBuffer[i++];
	  intLowByte  = writePortsBuffer[i++];
	  decHighByte = writePortsBuffer[i++];
	  decLowByte  = writePortsBuffer[i++];  
    if((temp1>>j)&0x01)	
    {
			sign = (intHighByte & 0x80)>>7;
	    value = (intHighByte&0x7f)*100 + intLowByte + decHighByte/100.0 + decLowByte/10000.0;	
			if(sign) 
				value = value * -1;	
			Registers[j+8] = value;
    }
  }
}

/*********************************************************************************************************
* Functoin name:       void PIDTunning(void)
* Descriptions:        PID�������ƣ���������Ч����24λ��12+12��
* input paraments:     ��
* output paraments:    ��    
* Returned values:     ��
*********************************************************************************************************/
void writePID(char* writePIDBuffer)
{
	int j,cnt,i = 0;
  char intHighByte[6],intLowByte[6],decHighByte[6],decLowByte[6],flag[6];
  float Kp,Ki,Kd,sp;
  char handle; //handle bit

  handle = writePIDBuffer[i++] & 0x0f;
  for(cnt=0;cnt<4;cnt++)
  {
		for(j=0;j<6;j++)
    {
			intHighByte[j]  = writePIDBuffer[i++];
			intLowByte[j]  = writePIDBuffer[i++];
			decHighByte[j] = writePIDBuffer[i++];
			decLowByte[j]  = writePIDBuffer[i++];
      flag[j] = (intHighByte[j] & 0x80)>>7;
    }
		
    Kp = (intHighByte[0]&0x7f)*100 + intLowByte[0] + decHighByte[0]/100.0 + decLowByte[0]/10000.0;
    Ki = (intHighByte[1]&0x7f)*100 + intLowByte[1] + decHighByte[1]/100.0 + decLowByte[1]/10000.0;
    Kd = (intHighByte[2]&0x7f)*100 + intLowByte[2] + decHighByte[2]/100.0 + decLowByte[2]/10000.0;
    sp = (intHighByte[3]&0x7f)*100 + intLowByte[3] + decHighByte[3]/100.0 + decLowByte[3]/10000.0;
    if(flag[0])
			Kp = Kp * (-1);
    if(flag[1]) 
			Ki = Ki * (-1);
    if(flag[2]) 
			Kd = Kd * (-1);
    if(flag[3]) 
			sp = sp * (-1);
		if((handle>>cnt) & 0x01)
	    PID_SetValue(cnt,Kp,Ki,Kd,sp);
	}
}

void readPID(char* readPIDBuffer)
{
   int i = 0;
   int j,c; 

   for(j=0;j<4;j++)		//AI1-AI12
   {
		 readPIDBuffer[i++] = getIntHighByte(PidPar[j].Kp);	    //intH
		 readPIDBuffer[i++] = getIntLowByte(PidPar[j].Kp);	   //intL
	   readPIDBuffer[i++] = getDecHighByte(PidPar[j].Kp);     //decH
	   readPIDBuffer[i++] = getDecLowByte(PidPar[j].Kp);	   //decL

	   readPIDBuffer[i++] = getIntHighByte(PidPar[j].Ki);	    //intH
	   readPIDBuffer[i++] = getIntLowByte(PidPar[j].Ki);	   //intL
	   readPIDBuffer[i++] = getDecHighByte(PidPar[j].Ki);     //decH
	   readPIDBuffer[i++] = getDecLowByte(PidPar[j].Ki);	   //decL

	   readPIDBuffer[i++] = getIntHighByte(PidPar[j].Kd);	    //intH
	   readPIDBuffer[i++] = getIntLowByte(PidPar[j].Kd);	   //intL
	   readPIDBuffer[i++] = getDecHighByte(PidPar[j].Kd);     //decH
	   readPIDBuffer[i++] = getDecLowByte(PidPar[j].Kd);	   //decL

	   readPIDBuffer[i++] = getIntHighByte(PidPar[j].sp);	    //intH
	   readPIDBuffer[i++] = getIntLowByte(PidPar[j].sp);	   //intL
	   readPIDBuffer[i++] = getDecHighByte(PidPar[j].sp);     //decH
	   readPIDBuffer[i++] = getDecLowByte(PidPar[j].sp);	   //decL

	   for(c=0;c<8;c++)
			readPIDBuffer[i++] = 0;
   }
}

/////////////////////////////////////////////////////////
///<��������> �ѵ��ź�ֵת���ɶ�Ӧ������ֵ
///<����1> ���ź�ֵ
///<����2> ת������	y = a*x + b
///<����ֵ> ��������ֵ
//////////////////////////////////////////////////////////
float getAIPhysicalValue(float eletricData,PARAMS par)
{   
	return  (eletricData * par.a + par.b);
}

float getAOPhysicalValue(float eletricData,PARAMS par)
{   
	return  (eletricData - par.b)/par.a;
}
