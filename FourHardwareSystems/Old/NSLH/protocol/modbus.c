void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
        if(BusyFlag)
            return;
        TIM_Cmd(TIM2,DISABLE);       //�رն�ʱ��2
        TIM_SetCounter(TIM2,0);      //�����ʱ
        Uart1_receive_buffer[Uart1_RCnt]=USART_RecieveData(USART1);       //��������
        //���ʱ������������µ�MODBUS���ݰ���ʼ����
        if(TimeOverFlag)
        {
            TimeOverFlag=0;
            BegainRxFlag=1;
            Uart1_RCnt++;
        }
        else if(BegainRxFlag)
        {
            Uart1_RCnt++;
        }
    }
    if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
    {
        if(Uart1_SCnt<Uart1_SendBuffSize)
	    {
	 	    USART_SendData(USART1,Uart1_Send_buffer[Uart1_SCnt++]);
	    }
        else
        {
            Uart1_SCnt=0;
            delay_ms(5);
            DE1=1;
            USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
        }
    }
    TIM_Cmd(TIM2, ENABLE);  //������ʱ��2
}

/*
Modbus ����CRCУ����Ĳ���
(1) Ԥ��16λ�Ĵ���ΪFFFFH���ƴ˼Ĵ���ΪCRC�Ĵ���
(2) �ѵ�һ��8λ������CRC�Ĵ����ĵ�λ����򣬰ѽ�������CRC�Ĵ���
(3) �ѼĴ�������������һλ(����λ)����0�����λ��������λ��
(4) ������λΪ0���ظ���3��(�ٴ���λ)��������λΪ1��CRC�Ĵ��������ʽA001(1010 0000 0000 0001) �������
(5) �ظ�����3��4��ֱ������8�Σ���������8λ����ȫ�������˴���
(6) �ظ�����2������5��������һ��8λ���ݵĴ���
(7) ���õ���CRC�Ĵ�����ΪCRC�� (CRC��=CRC_L+CRC_H)
*/
//CRC���ɺ���������CRC�洢��Ԥ�ȶ��ڵ������У����÷�ʽCRC16 (ָ�����ݵ�ָ�룬��ҪУ������ݳ���)
u16 crc16(u8 *ptr,u8 len)
{
    unsigned long wcrc=0xffff;       //Ԥ��16λCRC�Ĵ�������ֵȫ��Ϊ1
    int i=0,j=0;                     //�������
    for(i=0;i<len;i++)
    {
        wcrc^=*ptr++;            //��8λ������CRC�Ĵ������ָ���ַ���ӣ�ָ���¸�����
        for(j=0;j<8;j++)
        {
            if(wcrc&0x0001)          //�ж����Ƴ����ǲ���1�������1���������ʽ�������
            {
                wcrc=wcrc>>1^0Xa001;      //�Ƚ���������һλ��������Ķ���ʽ�������
            }
            else
            {
                wcrc>>=1;                //�������1��ֱ���Ƴ�
            }
        }
    }
    return wcrc<<8|wcrc>>8;       //��8λ��ǰ����8λ�ں�
}

void zh_USART_SendData(u8 ch)
{
   
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//??????????��???��????����?
    USART_SendData(USART1,ch);   
}

//�����ַ���
void zh_USART_SendString(u8* s,u8 len)
{
	  
    int i;
    for(i=0;i<len;i++)
    {
        zh_USART_SendData(s[i]);
    }
}

//����modbus
void DealModBus(void)
{
    u16 REG0X03[24];               //������0x03��Ӧ�ļĴ�����ַΪ0x0001-0x0019   12·AI+8·DI+4·RTD
    u16 REG0X06[16];               //������0x06��Ӧ�ļĴ�����ַΪ0x0001-0x0010   8·AO+8·DO
    u16 DatLength=0;
    u16 StartRegAdd=0;
    u8 i, sendLen;

    if(Uart1_receive_buffer[0]!=modbus_deviceId)           //�˶Ա�����ַ
    {
        return;
    }

    SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);   //�����У���
    SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];  //Ĭ�ϵĽ��յ���У����� �Ͱ�λ��ǰ���߰�λ�ں�
    if(SumCal!=SumRec)
    {
        return;
    }

    for(i=0;i<12;i++)                      //��ַ0x0001-0x000c
    {
        REG0X03[i]=AdREG[i];
    }
    
    for(i=0;i<4;i++)                      //��ַ0x000D-0x0011
    {
        REG0X03[12+i]=RTDdREG[i];
    }
    
    for(i=0;i<12;i++)                      //��ַ0x0012-0x0019
    {
        REG0X03[16+i]=DiREG[i];
    }
    
    for(i=0;i<8;i++)                      //��ַ0x0001-0x0008
    {
        REG0X06[i]=DAREG[i];
    }
    
    for(i=0;i<8;i++)                      //��ַ0x0009-0x0010
    {
        REG0X03[8+i]=DoREG[i];
    }

    StartRegAdd=Uart1_receive_buffer[2]*256+Uart1_receive_buffer[3];      //����ʵ�ʼĴ�����ַ
    DatLength=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];         //����Ĵ�����ַ����

    sendLen=0��
    Uart1_Send_buffer[sendLen++]=modbus_deviceId;
    Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[1];

    switch(Uart1_receive_buffer[1])
    {
        case 0x03:
            if((StartRegAdd+DatLength)>24) return;
            Uart1_Send_buffer[sendLen++]=DatLength*2;
            for(i=0;i<DatLength;i++)                                             //��ȡ���������Ĵ���
            {
                Uart1_Send_buffer[sendLen++]=REG0X03[StartRegAdd+i]>>8;
                Uart1_Send_buffer[sendLen++]=REG0X03[StartRegAdd+i];
            }
            break;
        case 0x06:
            if(StartRegAdd>12) return;
            REG0X06[StartRegAdd]=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
            break;
        default:          //��֧�ֹ�����
            Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
            Uart1_Send_buffer[1]=0x80;                  //���������λ��1
            Uart1_Send_buffer[2]=0x01;                  //�����쳣��Ϊ0x01---��Ч����
            break;
    }

    SumCal=crc16(&Uart1_Send_buffer[0],sendLen);            //�����У���
    Uart1_Send_buffer[sendLen++]=SumCal>>8;                 //CRC���ֽ�
    Uart1_Send_buffer[sendLen++]=SumCal&0xff;               //CRC���ֽ�

    RS485MOED_S��                                           //485����
    zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);    //������Ӧ֡
    delay_nms(2);
    RS485MOED_R;                                           //485����

    for(i=0;i<8;i++)                                        //DA�Ĵ�����ַΪ0x0001----0x0009
    {
        DAREG[i]=REG0X06[i];
    }

    for(i=0;i<8;i++)                                        //DO�Ĵ�����ַΪ0x000a---0x0010
    {
        DoREG[i]=REG0X06[8+i];
    }
}


void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        TimerOverFlag=1;
        if(BegainRxFlag==1)
        {
            BegainRxFlag=0;
            BusyFlag=1;
            DealModBus();
            Uart1_RCnt=0;
            BusyFlag=0;
        }
    }
}