#include "modbusNew.h"
#include "Globle.h"

#include "modbus.h"

#define DeviceId  0x01

u16 startRegAddr;
u16 RegNum;
u16 calCRC;

 u8 auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ; 

const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;

void Modbus_RegMap(void)
{
   //���뿪�����Ĵ���ָ��
   Modbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4):ȡPE4��ַ,(vu32*)&PEin(4)��PE4��ַǿ��ת����uw32���͵ĵ�ַ,Modbus_InputIO[0]=(vu32*)&PEin(4); ��ת���õĵ�ַ�͸���ַָ��Modbus_InputIO[0];
   Modbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*Modbus_InputIO[0] ȡ����ַ�е�����
   Modbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
   Modbus_InputIO[3]=(vu32*)&PAin(0);//KEY3

   //����������Ĵ���ָ��
   Modbus_OutputIO[0]=(vu32*)&PFout(9);//LED0
   Modbus_OutputIO[1]=(vu32*)&PFout(10);//LED1

  //���ּĴ���ָ��
   Modbus_HoldReg[0]=(u16*)&tempData;     
}

//����У����
u16 CRC_Compute(u8 *puchMsg, u16 usDataLen) 
{
	u8 uchCRCHi = 0xFF ; 
  u8 uchCRCLo = 0xFF ; 
  u32 uIndex ; 
  while (usDataLen--) 
		{
			uIndex = uchCRCHi ^ *puchMsg++ ; 
      uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
      uchCRCLo = auchCRCLo[uIndex] ; 
    } 
	return ((uchCRCHi<< 8)  | (uchCRCLo)) ; 
}

//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len:���͵��ֽ���
void RS485_SendData(u8 *buff,u8 len)
{
	u8 t;
  DE1=0;//�л�����ģʽ
  for(t=0;t<len;t++)
  {
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ�������Ϊ��
    USART_SendData(USART1,buff[t]);
  }
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//��ɷ���
}

//RS485�������,���ڴ�����յ�������(����������ѭ������)
void RS485_Service(void)
{
	u16 recCRC;                          //�յ���У����
  if(RS485_FrameFlag==1)
  {
		if(RS485_RX_BUFF[0]==DeviceId)//��ַ��ȷ
			{
				if((RS485_RX_BUFF[1]==01)||(RS485_RX_BUFF[1]==02)||(RS485_RX_BUFF[1]==03)||(RS485_RX_BUFF[1]==05)||(RS485_RX_BUFF[1]==06)||(RS485_RX_BUFF[1]==15)||(RS485_RX_BUFF[1]==16))//��������ȷ
					{
						startRegAddr=(((u16)RS485_RX_BUFF[2])<<8)|RS485_RX_BUFF[3];//��ȡ�Ĵ�����ʼ��ַ
						if(startRegAddr<1000)//�Ĵ�����ַ�ڷ�Χ��
							{
								calCRC=CRC_Compute(RS485_RX_BUFF,RS485_RX_CNT-2);//������յ�CRC
								recCRC=RS485_RX_BUFF[RS485_RX_CNT-1]|(((u16)RS485_RX_BUFF[RS485_RX_CNT-2])<<8);//���յ���CRC(���ֽ���ǰ,���ֽ��ں�)
								if(calCRC==recCRC)//CRCУ����ȷ
									{
										switch(RS485_RX_BUFF[1])     //���ݲ�ͬ�Ĺ�������д���
											{
												case 1://����������
													{
														Modbus_01_Solve();
														break;
													}
												case 2://�����뿪����
													{
														Modbus_02_Solve();
														break;
													}
												case 5://д�������������
													{
														Modbus_05_Solve();
														break;
													}
												case 15://д������������
													{
														Modbus_15_Solve();
														break;
													}
												case 03: //������Ĵ���
													{                                                               
														Modbus_03_Solve();
														break;
													}
												case 06: //д�����Ĵ���
													{
														Modbus_06_Solve();
														break;
													}
												case 16: //д����Ĵ���
													{
														Modbus_16_Solve();
														break;
													}
												}
										}
									else//CRCУ�����
										{
											RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
											RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
											RS485_TX_BUFF[2]=0x04; //�쳣��
											RS485_SendData(RS485_TX_BUFF,3);
										}        
								}
							else//�Ĵ�����ַ������Χ
								{
									RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
									RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
									RS485_TX_BUFF[2]=0x02; //�쳣��
									RS485_SendData(RS485_TX_BUFF,3);
								}                                                
						}
					else//���������
						{
							RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
							RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
							RS485_TX_BUFF[2]=0x01; //�쳣��
							RS485_SendData(RS485_TX_BUFF,3);
						}
				}
			
			RS485_FrameFlag=0;                         //��λ֡������־
			RS485_RX_CNT=0;                            //���ռ���������
			DE1=1;                                     //��������ģʽ                
	 }                
}

//Modbus������02�������/////////////////////////////////////////////////////��������֤OK -----����������PE4 PE3 PE2 PA0 ��ʼ�������ſ���OK    KEY_Init();
//�����뿪����
void Modbus_02_Solve(void)
{
	u16 ByteNum;
  u16 i;
  RegNum= (((u16)RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//��ȡ�Ĵ�������
  if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
  {
		RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
    RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
    ByteNum=RegNum/8;//�ֽ���
    if(RegNum%8) ByteNum+=1;//���λ����������,���ֽ���+1
    RS485_TX_BUFF[2]=ByteNum;//����Ҫ��ȡ���ֽ���
    for(i=0;i<RegNum;i++)
    {
			if(i%8==0) RS485_TX_BUFF[3+i/8]=0x00;
      RS485_TX_BUFF[3+i/8]>>=1;//��λ�ȷ���
      RS485_TX_BUFF[3+i/8]|=((*Modbus_InputIO[startRegAddr+i])<<7)&0x80;
      if(i==RegNum-1)//���͵����һλ
      {
				if(RegNum%8) RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�������,��ʣ��MSB���0
      }
    }
    calCRC=CRC_Compute(RS485_TX_BUFF,ByteNum+3);
    RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
    RS485_SendData(RS485_TX_BUFF,ByteNum+5);
  }
  else              //�Ĵ�����ַ+����������Χ
  {
		RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
    RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
    RS485_TX_BUFF[2]=0x02; //�쳣��
    RS485_SendData(RS485_TX_BUFF,3);
  }
}

//Modbus������01????������� ///////////////////////////////////////////////////////////OK
//�����������
void Modbus_01_Solve(void)
{
        u16 ByteNum;
        u16 i;
        RegNum= (((u16)RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
                RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
                ByteNum=RegNum/8;//�ֽ���
                if(RegNum%8) ByteNum+=1;//���λ����������,���ֽ���+1
                RS485_TX_BUFF[2]=ByteNum;//����Ҫ��ȡ���ֽ���
                for(i=0;i<RegNum;i++)
                {
                        if(i%8==0) RS485_TX_BUFF[3+i/8]=0x00;
                        RS485_TX_BUFF[3+i/8]>>=1;//��λ�ȷ���
                        RS485_TX_BUFF[3+i/8]|=((*Modbus_OutputIO[startRegAddr+i])<<7)&0x80;
                        if(i==RegNum-1)//���͵����һλ��
                        {
                                if(RegNum%8) RS485_TX_BUFF[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�������,��ʣ��MSB���0
                        }
                }
                calCRC=CRC_Compute(RS485_TX_BUFF,ByteNum+3);
                RS485_TX_BUFF[ByteNum+3]=(calCRC>>8)&0xFF;
                RS485_TX_BUFF[ByteNum+4]=(calCRC)&0xFF;
                RS485_SendData(RS485_TX_BUFF,ByteNum+5);
        }
        else    //�Ĵ�����ַ+����������Χ
        {
                RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
                RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
                RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(RS485_TX_BUFF,3);
        }
}

//Modbus������05�������   ///////////////////////////////////////////////////////OK
//д�������������
void Modbus_05_Solve(void)
{
        if(startRegAddr<100)//�Ĵ�����ַ�ڷ�Χ��
        {
                if((RS485_RX_BUFF[4]==0xFF)||(RS485_RX_BUFF[5]==0xFF)) *Modbus_OutputIO[startRegAddr]=0x01;
                else *Modbus_OutputIO[startRegAddr]=0x00;

                RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
                RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
                RS485_TX_BUFF[2]=RS485_RX_BUFF[2];
                RS485_TX_BUFF[3]=RS485_RX_BUFF[3];
                RS485_TX_BUFF[4]=RS485_RX_BUFF[4];
                RS485_TX_BUFF[5]=RS485_RX_BUFF[5];

                calCRC=CRC_Compute(RS485_TX_BUFF,6);
                RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
                RS485_TX_BUFF[7]=(calCRC)&0xFF;
                RS485_SendData(RS485_TX_BUFF,8);
        }
        else//�Ĵ�����ַ������Χ
        {
                RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
                RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
                RS485_TX_BUFF[2]=0x02; //???
                RS485_SendData(RS485_TX_BUFF,3);
        }
}

//Modbus������15   //////////////////////////////////////////////////////OK
//д������������
void Modbus_15_Solve(void)
{
        u16 i;
        RegNum=(((u16)RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//???????
        if((startRegAddr+RegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {        
                for(i=0;i<RegNum;i++)
                {
                        if(RS485_RX_BUFF[7+i/8]&0x01) *Modbus_OutputIO[startRegAddr+i]=0x01;
                        else *Modbus_OutputIO[startRegAddr+i]=0x00;
                        RS485_RX_BUFF[7+i/8]>>=1;//�ӵ�λ��ʼ
                }

                RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
                RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
                RS485_TX_BUFF[2]=RS485_RX_BUFF[2];
                RS485_TX_BUFF[3]=RS485_RX_BUFF[3];
                RS485_TX_BUFF[4]=RS485_RX_BUFF[4];
                RS485_TX_BUFF[5]=RS485_RX_BUFF[5];
                calCRC=CRC_Compute(RS485_TX_BUFF,6);
                RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
                RS485_TX_BUFF[7]=(calCRC)&0xFF;
                RS485_SendData(RS485_TX_BUFF,8);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
                RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
                RS485_TX_BUFF[2]=0x02; //�쳣��
                RS485_SendData(RS485_TX_BUFF,3);
        }
}

//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////OK
//�����ּĴ���
void Modbus_03_Solve(void)
{
        u8 i;
        RegNum= (((u16)RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//��ȡ�Ĵ�������
        if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
        {
					RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
          RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
          RS485_TX_BUFF[2]=RegNum*2;
          for(i=0;i<RegNum;i++)
          {
            RS485_TX_BUFF[3+i*2]=(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//     �ȷ��͸��ֽ��ڷ��͵��ֽ�
						RS485_TX_BUFF[4+i*2]=(*Modbus_HoldReg[startRegAddr+i])&0xFF; //
          }
          calCRC=CRC_Compute(RS485_TX_BUFF,RegNum*2+3);
          RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;         //CRC???????  // ????
          RS485_TX_BUFF[RegNum*2+4]=(calCRC)&0xFF;
					RS485_SendData(RS485_TX_BUFF,RegNum*2+5);
        }
        else//�Ĵ�����ַ+����������Χ
        {
          RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
          RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
          RS485_TX_BUFF[2]=0x02; //�쳣��
          RS485_SendData(RS485_TX_BUFF,3);
        }
}


//Modbus������06�������   //////////////////////////////////////////////////////////////////////////////////OK
//д�������ּĴ���
void Modbus_06_Solve(void)
{
	*Modbus_HoldReg[startRegAddr]=RS485_RX_BUFF[4]<<8;//���ֽ���ǰ              �޸�Ϊ���ֽ���ǰ�����ֽ��ں�
  *Modbus_HoldReg[startRegAddr]|=((u16)RS485_RX_BUFF[5]);//���ֽ��ں�

  RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
  RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
  RS485_TX_BUFF[2]=RS485_RX_BUFF[2];
  RS485_TX_BUFF[3]=RS485_RX_BUFF[3];
  RS485_TX_BUFF[4]=RS485_RX_BUFF[4];
  RS485_TX_BUFF[5]=RS485_RX_BUFF[5];

  calCRC=CRC_Compute(RS485_TX_BUFF,6);
  RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
  RS485_TX_BUFF[7]=(calCRC)&0xFF;
  RS485_SendData(RS485_TX_BUFF,8);
}

//Modbus������16������� /////////////////////////////////////////////////////////////////////////////////////////////////?????OK
//д������ּĴ���
void Modbus_16_Solve(void)
{
	u8 i;
  RegNum= (((u16)RS485_RX_BUFF[4])<<8)|((RS485_RX_BUFF[5]));//��ȡ�Ĵ�������
  if((startRegAddr+RegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
  {
		for(i=0;i<RegNum;i++)
    {
			*Modbus_HoldReg[startRegAddr+i]=RS485_RX_BUFF[7+i*2]<<8; //���ֽ���ǰ            ���ֽ���ǰ�����ֽ��ں�
      *Modbus_HoldReg[startRegAddr+i]|=((u16)RS485_RX_BUFF[8+i*2]); //���ֽ��ں�
    }

    RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
    RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
    RS485_TX_BUFF[2]=RS485_RX_BUFF[2];
    RS485_TX_BUFF[3]=RS485_RX_BUFF[3];
    RS485_TX_BUFF[4]=RS485_RX_BUFF[4];
    RS485_TX_BUFF[5]=RS485_RX_BUFF[5];

    calCRC=CRC_Compute(RS485_TX_BUFF,6);
    RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
    RS485_SendData(RS485_TX_BUFF,8);
  }
  else//�Ĵ�����ַ+����������Χ
   {
		 RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
     RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
     RS485_TX_BUFF[2]=0x02; //�쳣��
     RS485_SendData(RS485_TX_BUFF,3);
   }
}

