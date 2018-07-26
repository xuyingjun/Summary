void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
        if(BusyFlag)
            return;
        TIM_Cmd(TIM2,DISABLE);       //关闭定时器2
        TIM_SetCounter(TIM2,0);      //清除计时
        Uart1_receive_buffer[Uart1_RCnt]=USART_RecieveData(USART1);       //连续发送
        //如果时间溢出，代表新的MODBUS数据包开始发送
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
    TIM_Cmd(TIM2, ENABLE);  //开启定时器2
}

/*
Modbus 计算CRC校验码的步骤
(1) 预制16位寄存器为FFFFH，称此寄存器为CRC寄存器
(2) 把第一个8位数据与CRC寄存器的地位相异或，把结果存放与CRC寄存器
(3) 把寄存器的内容右移一位(朝低位)，用0来填补高位，检查最低位。
(4) 如果最低位为0：重复第3步(再次移位)，如果最低位为1：CRC寄存器与多项式A001(1010 0000 0000 0001) 进行异或
(5) 重复步骤3和4，直到右移8次，这样整个8位数据全部进行了处理
(6) 重复步骤2到步骤5，进行下一个8位数据的处理
(7) 最后得到的CRC寄存器即为CRC码 (CRC码=CRC_L+CRC_H)
*/
//CRC生成函数，并将CRC存储在预先定于的数组中，调用方式CRC16 (指向数据的指针，需要校验的数据长度)
u16 crc16(u8 *ptr,u8 len)
{
    unsigned long wcrc=0xffff;       //预制16位CRC寄存器，初值全部为1
    int i=0,j=0;                     //定义计数
    for(i=0;i<len;i++)
    {
        wcrc^=*ptr++;            //将8位数据与CRC寄存器异或，指针地址增加，指向下个数据
        for(j=0;j<8;j++)
        {
            if(wcrc&0x0001)          //判断右移出的是不是1，如果是1，则与多项式进行异或
            {
                wcrc=wcrc>>1^0Xa001;      //先将数据右移一位再与上面的多项式进行异或
            }
            else
            {
                wcrc>>=1;                //如果不是1，直接移出
            }
        }
    }
    return wcrc<<8|wcrc>>8;       //低8位在前，高8位在后
}

void zh_USART_SendData(u8 ch)
{
   
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//??????????×???·????ê±?
    USART_SendData(USART1,ch);   
}

//发送字符串
void zh_USART_SendString(u8* s,u8 len)
{
	  
    int i;
    for(i=0;i<len;i++)
    {
        zh_USART_SendData(s[i]);
    }
}

//处理modbus
void DealModBus(void)
{
    u16 REG0X03[24];               //功能码0x03对应的寄存器地址为0x0001-0x0019   12路AI+8路DI+4路RTD
    u16 REG0X06[16];               //功能码0x06对应的寄存器地址为0x0001-0x0010   8路AO+8路DO
    u16 DatLength=0;
    u16 StartRegAdd=0;
    u8 i, sendLen;

    if(Uart1_receive_buffer[0]!=modbus_deviceId)           //核对本机地址
    {
        return;
    }

    SumCal=crc16(&Uart1_receive_buffer[0],Uart1_RCnt-2);   //计算出校验和
    SumRec=(Uart1_receive_buffer[Uart1_RCnt-2]<<8)+Uart1_receive_buffer[Uart1_RCnt-1];  //默认的接收到的校验和是 低八位在前，高八位在后
    if(SumCal!=SumRec)
    {
        return;
    }

    for(i=0;i<12;i++)                      //地址0x0001-0x000c
    {
        REG0X03[i]=AdREG[i];
    }
    
    for(i=0;i<4;i++)                      //地址0x000D-0x0011
    {
        REG0X03[12+i]=RTDdREG[i];
    }
    
    for(i=0;i<12;i++)                      //地址0x0012-0x0019
    {
        REG0X03[16+i]=DiREG[i];
    }
    
    for(i=0;i<8;i++)                      //地址0x0001-0x0008
    {
        REG0X06[i]=DAREG[i];
    }
    
    for(i=0;i<8;i++)                      //地址0x0009-0x0010
    {
        REG0X03[8+i]=DoREG[i];
    }

    StartRegAdd=Uart1_receive_buffer[2]*256+Uart1_receive_buffer[3];      //计算实际寄存器地址
    DatLength=Uart1_receive_buffer[4]*256+Uart1_receive_buffer[5];         //计算寄存器地址长度

    sendLen=0；
    Uart1_Send_buffer[sendLen++]=modbus_deviceId;
    Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[1];

    switch(Uart1_receive_buffer[1])
    {
        case 0x03:
            if((StartRegAdd+DatLength)>24) return;
            Uart1_Send_buffer[sendLen++]=DatLength*2;
            for(i=0;i<DatLength;i++)                                             //读取单个或多个寄存器
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
        default:          //不支持功能码
            Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[2];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[3];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[4];
			Uart1_Send_buffer[sendLen++]=Uart1_receive_buffer[5];
            Uart1_Send_buffer[1]=0x80;                  //功能码最高位置1
            Uart1_Send_buffer[2]=0x01;                  //设置异常码为0x01---无效功能
            break;
    }

    SumCal=crc16(&Uart1_Send_buffer[0],sendLen);            //计算出校验和
    Uart1_Send_buffer[sendLen++]=SumCal>>8;                 //CRC低字节
    Uart1_Send_buffer[sendLen++]=SumCal&0xff;               //CRC高字节

    RS485MOED_S；                                           //485发送
    zh_USART_SendString(&Uart1_Send_buffer[0],sendLen);    //发出响应帧
    delay_nms(2);
    RS485MOED_R;                                           //485接收

    for(i=0;i<8;i++)                                        //DA寄存器地址为0x0001----0x0009
    {
        DAREG[i]=REG0X06[i];
    }

    for(i=0;i<8;i++)                                        //DO寄存器地址为0x000a---0x0010
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