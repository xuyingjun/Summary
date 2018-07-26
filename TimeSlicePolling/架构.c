//主程序架构
void main()
{
	Init_All();   //初始化函数
	while(1)      //主循环函数
	{
		TimeEvent();      //事件驱动函数
		RxProcess();      //串口接收数据处理
		TxProcess();      //串口发送数据处理		
	}
	
}                         //主程序结束


/**********************************************
               系统基准节拍
               Input:NONE
               Output:Return KeyCode
**********************************************/
void Timer0Interrupt(void)
{
	TH0=0x0fc;
	TL0=0x66; 
	if((++Time_1ms&&0x03)==0)
		TimeIntFlag=1;
}

//时间事件，时间轮询法调用函数
void TimeEvent(void)
{
	if(TimeIntFlag)
	{
		TimeIntFlag=0;
		ClearWatchDog();    //这里喂狗
		Display();          //在4ms事件中，通过LED扫描显示，以及喂狗
		if(++Time20ms==5)
		{
			Time20ms=0;
			TimeEvent20ms();        //在20ms事件中，处理键盘扫描函数
		}
		
		if(++Time1s==10)
		{
			Time1s=0;
			TimeEvent1s();          //在1s事件中，使工作指示灯闪烁
			
		}
		
		UARTTimeEvent();   //串口的数据接收事件在4ms事件中处理
	}
	
}