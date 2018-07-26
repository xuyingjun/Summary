#include "protocol.h"

////////////////////////////////////////////////////
///<��������>�������յ������ݵİ�ͷ
///<����> ָ�룬ָ����ջ���Ĵ����׵�ַ
///<˵��> ��ͷ��10���ֽڹ���
///<����ֵ>null
///////////////////////////////////////////////////
/************************************************************************
1-4����ַλ����Ϊ���ư��ַ�Ͷ˿ڵ�ַ��1-���Ʊ���ֽڣ�2-���Ʊ���ֽڣ�3-4���˿ڵ�ַΪ0
5��ģʽλ; �̶�λ0x02,����ư��й�
6��������; ���˿�(0x55),д�˿�(0x66),��PID(0x77),дPID(0x88)
7��У��λ; ���ݶ���ͣ�Ȼ����ܺ�ȡ100������
8-9�����ݳ���; 2���ֽڱ�ʾ�����ֽڵĳ��ȣ���λ��ǰ����λ�ں�
10������λ; �̶���ֵΪ0
************************************************************************/
int PacketHeaderAnalysis(char *data)
{
	int length;
  length = data[7]*100 + data[8];       //������ݶγ���
	//�ж�ģʽλ
	if(data[4] == 0x01)                   //ģʽΪ0x01
		{
			MCUaddress = data[0]*100 + data[1];  //������ư��ַ
			StageSel =  CONFIG;                  //״̬ѡ��
			ModeSel = (MODESEL)data[5];          //����ѡ��
			LineSel = (LINESEL)data[9];          //online---offline
		}
  else if(data[4] == 0x02)              //ģʽΪ0x02
		{
			StageSel =  RUNTIME;
			TaskSel  =  (TASKSEL)data[5];
		}
  
	return length;                        //�������ݶ��ֽڳ���
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

//�����������ݣ���ͷ�����ݰ�����β
//length:���ݶγ���,dataBuffer��Ϊ���ݶ����ݣ� sentBuffer:���ͻ��棬������ͷ�����ݶκͰ�β
void SentDataGeneration(int length,char* dataBuffer,char* sentBuffer)
{
	int i = 0,j;

	//�������ݷ���ͷ�ļ�
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
	
	//������ݶ�
  for(j=0;j<length;j++)
	{
		sentBuffer[i++] = dataBuffer[j];
  }
   
	sentBuffer[i++] = 0xff;
  sentBuffer[i++] = 0xff;
}

////////////////////////////////////////////////////
///<��������>У����յ������ݶ������Ƿ���ȷ
///<����1> ָ�룬ָ����ջ���Ĵ����׵�ַ(��ͷ����)
///<����ֵ> 0 -- ���ݴ���ʧ�棬1--���ݴ�����ȷ
///////////////////////////////////////////////////
int CheckReceivedData(char* receivedBuffer)
{
	int i; 
  long result = 0;		                   //�ۼӺ�
  char calculatedCheckCode;  	           //������У����
  char receivedCheckCode;                //���յ�У����
  int dataLength;                        //���ݶ����ݵĳ���
  unsigned int address;                  //��ַ��У��
	
	//�ж�ģʽλ���̶�λ0x02
	if(receivedBuffer[4] == 0x02)
		{
		//�жϿ�������ַ
			address = receivedBuffer[0]*100 + receivedBuffer[1];
			if( MCUaddress != address)  
				return 0;
		}
	
	//�洢���յ���У����
	receivedCheckCode = receivedBuffer[6];
	//����������ݳ��ȣ����ݶγ���
  dataLength = receivedBuffer[7]*100 + receivedBuffer[8];
	//�����������У���
  for(i=0;i<dataLength;i++)
  {
		result += receivedBuffer[i+LENGTH];
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

//�õ������������ָ��ֽ�λ
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

//�õ������������ֵ��ֽ�λ
char getIntLowByte(float data)
{
	char lowByte = 0;
  int temp = (int)data;
  if(temp<0)
		temp = temp * (-1);
	lowByte = temp % 100;
  return lowByte;
}

//�õ�����С�����ָ��ֽ�����
char getDecHighByte(float data)
{
	char decByte = 0;
	if(data<0)
		data = data * (-1);
	decByte = ((int)(data*10000) % 10000) / 100;
	return decByte;
}

//�õ�����С�����ֵ��ֽ�����
char getDecLowByte(float data)
{
    char decByte = 0;
	if(data<0)
	   data = data * (-1);
	decByte = (int)(data*10000) % 100;
	return decByte;
}
