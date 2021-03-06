#include "stm32_config.h"
#include "modbus.h"
#include "stm32f10x_iwdg.h"

u32 data1=0;
//printf打印到串口中
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
void normal(void);
	
float dataTemp[12];
float RTDtemperature[4];
float datavalue[8]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0};
float AI_Input[12]={0};


//定义全局变量
u8 RS485_RX_BUFF[128];               //接收缓存区2048字节
u16 RS485_RX_CNT=0;                   //接收计数器
u8 RS485_FrameFlag=0;                 //帧结束标记
u8 RS485_TX_BUFF[128];               //发送缓存区2048字节
u16 RS485_TX_CNT=0;                   //发送计数器

//Modbus寄存器和单片机寄存器映射关系
vu32 *Modbus_InputIO[100];            //输入开关量寄存器指针(这里使用的是位带操作)
vu32 *Modbus_OutputIO[100];           //输出开关量寄存器指针(这里使用的是位带操作)
u16  *Modbus_HoldReg[128];            //保持寄存器
u8 DataFlag=0;
int main(void)
{
 	RCC_Configuration();            //系统时钟配置信息
	IO_Configuration();             //DI and DO 初始化
	ADC_init();                     //AI初始化
	DAC_init();                     //AO初始化
	RTD_init();                     //测温初始化
	Usart_Total_Initi();            //串口初始化
	Timer1Config();                 //定时处理大板所有端口数据
	Tim2Configure();               	//定时处理与小板通信数据
	Tim4Configure();               	//定时器4定时15s，定时处理PID control algorithm,时间间隔是15s
	
	InitializeTimer(2,2);          	//初始化定时器3，定时2s，定时处理端口数据
	
	InitialSys();                   //系统初始化，主要是进行端口初始化
	IWDG_Configuration();           //看门狗初始化配置
	
	while(1)
	{
		IWDG_ReloadCounter();         //喂狗  10s
		ValueTran();                  //部分参数转换，主要是处理与Modbus通信的数据
		Modbus_RegMap();              //modbus通信数据，地址数据处理
		RS485_Service();              //modbus通信处理

		//14：阀紧急全开命令，不执行正常温度控制逻辑
		if(ValveCmd!=0&&ForceValveFlag==0)
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//开ptb风扇
			APortOut(PTBFan2,8.0);		//开ptb风扇
			ForceValveFlag=1;
		}
		else if(ValveCmd==0&&ForceValveFlag==1)
		{
			APortOut(CHWV,0.0);
			APortOut(PTBFan1,0.0);		//开ptb风扇
			APortOut(PTBFan2,0.0);		//开ptb风扇
			ForceValveFlag=0;
		}
		else if(ValveCmd!=0&&ForceValveFlag==1)
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//开ptb风扇
			APortOut(PTBFan2,8.0);		//开ptb风扇
			ForceValveFlag=1;
		}
		
		//DPortOut(ERVFan,1);
		
		//有了ERV强制启动命令，不管当前ERV处于什么状态，都需要开启ERV
		if(ERVCmd!=0)
		{
			DPortOut(ERVFan,1);
			ERVState=1;
			ERVFlag=1;
		}
		if(ERVCmd==0&&ERVFlag==1)   //在ERV强制启动后，ERVCmd命令为0，即关闭ERV通风
		{
			DPortOut(ERVFan,0);
			ERVState=0;
			ERVFlag=0;
		}
		
		//没有ERV强制命令，同时处于非强制模式，根据CO2浓度进行控制
		if(ERVCmd==0&&ERVFlag==0)
		{
			CO2Control();//非紧急的正常运行，CO2控制逻�
		}
		
		normal();	
	}
}	

//normal logic
void normal(void)
{
			//RunStatus为系统运行状态标志位，RunStatus--0:系统未运行，RunStatus--1：系统正常运行
		if(!RunStatus&&(IBSCommand||ScheduleComm||PDVComm))      //收到Booking或者Schedule命令启动系统
		{
			//系统启动过程中，判断是否强制开阀
			if(ValveCmd!=0)
			{
				if(ValveCmd>100)
					ValveCmd=100;
				if(ValveCmd<1)
					ValveCmd=0;
				APortOut(CHWV,ValveCmd/10.0);
				APortOut(PTBFan1,8.0);		//开ptb风扇
				APortOut(PTBFan2,8.0);		//开ptb风扇
				ForceValveFlag=1;
				return;
			}
			else if(ValveCmd==0&&ForceValveFlag==1)
			{
				APortOut(CHWV,0.0);
				APortOut(PTBFan1,0.0);		//开ptb风扇
				APortOut(PTBFan2,0.0);		//开ptb风扇
				ForceValveFlag=0;
			}
			else if(ValveCmd==0&&ForceValveFlag==0)
			{
				AlarmFlag=NSLH_Start();              					//系统启动，水阀开40%
			}
			
			if(AlarmFlag!=0)                              //如果系统启动异常，关闭所有端口输出信号
				NSLH_Clear();
			RunStatus=1;
		}

		//系统正在运行过程中，收到强制开阀命令
		if(RunStatus&&(ValveCmd!=0))
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//开ptb风扇
			APortOut(PTBFan2,8.0);		//开ptb风扇
			ForceValveFlag=1;
		}
		
		//系统运行时，收到E-booking或者Schedule关系统指令时，关闭系统
		if(RunStatus&&((!IBSCommand)&&(!ScheduleComm)&&(!PDVComm)))
		{
			if(ForceValveFlag==1&&(ValveCmd!=0))
			{
				if(ValveCmd>100)
					ValveCmd=100;
				if(ValveCmd<1)
					ValveCmd=0;
				APortOut(CHWV,ValveCmd/10.0);
				APortOut(PTBFan1,8.0);		//开ptb风扇
				APortOut(PTBFan2,8.0);		//开ptb风扇
				ForceValveFlag=1;
				return;
			}
			else if(ValveCmd==0&&ForceValveFlag==1)
			{
				APortOut(CHWV,0.0);
				APortOut(PTBFan1,0.0);		//关ptb风扇
				APortOut(PTBFan2,0.0);		//关ptb风扇
				ForceValveFlag=0;
			}
			else if(ValveCmd==0&&ForceValveFlag==0)
			{
				AlarmFlag=NSLH_Stop();                      //关闭系统
			}	
			if(AlarmFlag!=0)
			{
				NSLH_Clear();                             //如果系统异常，关闭所有端口
			}
			RunStatus=0;
		}
		
		//在系统正常运行状态下，Motion Sensor检测到有人进入房间
		if(RunStatus&&OccSta&&ForceValveFlag==0)
		{
			if(RoomTemp==0&&RoomHum==0&&CO2==0)
			{
				APortOut(CHWV,10.0);
				DataFlag=1;
			}
			else
			{
				DataFlag=0;
			}
			if(RoomTempSv==250)
			{
				RoomTempSv=230;
			}
			if(!DataFlag)
			{
				AlarmFlag=OccupanyMode();     //执行有人情况下控制操作
				if(AlarmFlag!=0)              //如果系统异常，则关闭全部端口信息
				{
					NSLH_Clear();
				}	
			}
		}
		
		//在系统正常运行状态下，Motion Sensor检测到房间无人状态下
		if(RunStatus&&(!OccSta)&&ForceValveFlag==0)
		{
			RoomTempSv=250;              //无人状况下，房间温度设定值为27度，保持不变
			TempSPSet_Unocc=250;
			if(RoomTemp==0&&RoomHum==0&&CO2==0)
			{
				APortOut(CHWV,10.0);
				DataFlag=1;
			}
			else
			{
				DataFlag=0;
			}
			
			if(!DataFlag)
			{
				AlarmFlag=Unoccupanied();    //执行无人模式下，温度控制操作，温度设定值为27度，PID调节水阀
				if(AlarmFlag!=0)
				{
					NSLH_Clear();
				}
			}
		}
}


void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                              //定义外部高速晶体启动状态枚举变量；
	RCC_DeInit();                                              //复位RCC寄存器到默认值；
	RCC_HSEConfig(RCC_HSE_ON);                                 //打开外部的高速晶振； 
	HSEStartUpStatus = RCC_WaitForHSEStartUp();                //等待外部高速时钟准备好
	if(HSEStartUpStatus == SUCCESS)
		{
			RCC_HCLKConfig(RCC_SYSCLK_Div1);                         //配置AHB
			RCC_PCLK2Config(RCC_HCLK_Div1);                          //配置APB2 
			RCC_PCLK1Config(RCC_HCLK_Div2);                          //配置APB1 
			//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	      
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //PLL=8M*9=72M
			RCC_PLLCmd(ENABLE);                                      //时能PLL
			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);      //等待PLL就绪	      
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);               //配置系统时钟=PLL	  
			while(RCC_GetSYSCLKSource() != 0x08);                    //检查PLL时钟是否作为系统时钟	       
		}
}

PUTCHAR_PROTOTYPE    //使用printf函数时，可以将数据通过串口打印出来
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(u8)ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}

void InitialSys(void)
{
	DPortCIn(DI_data);  //Read the Status motion sensor(DI0) and 2 Damper(DI7 and DI6) 
	RTD_allchannels(RTDtemperature);  //Read the return water temperature (RTD 0)
	APortCIn(dataTemp);	//Read the DP sensor value(AI3) and chilled water valve feedback(AI11)
}

void ValueTran(void)
{
	RoomTempPv=(u16)(RoomTemp*10);
	RoomTempSetValue=(RoomTempSv*10);
	RoomHumPv=(RoomHum*10);
	RoomHumSetValue=(60*10);
	CHWTemp=(RTDtemperature[RTDchannel]*10+0.5);
	CO2Value=CO2;
	CO2ValueSv=CO2Sv;
	
	//小房间
	OccSta=DI_data[0]*10;
	
	//大房间，两个101
	//OccSta=(DI_data[0]|DI_data[1])*10;
	
	//其他房间 阀门反馈值才有这段代码
	ValveFb=(AI_Input[CHWVfb]*10+0.5);
	
	//如果房间号为NNSLH1L106,阀门反馈使用该段代码，同时屏蔽掉上段代码
//	if(ValveCmd!=0)
//	{
//		ValveFb=ValveCmd;
//	}
//	else if(ValveCmd==0)
//	{
//		ValveFb=ValvePosition*10;
//	}
	
	DPValue=(AI_Input[3]*10);
	
	//读取阀门反馈
	APortIn(CHWVfb,AI_Input);
	//读取压差反馈
	APortIn(3,AI_Input);
}

void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //写入0x5555，用于允许寄存器写入功能，解除寄存器保护
	IWDG_SetPrescaler(IWDG_Prescaler_256);     //时钟分频 40K/256=156HZ(6.4ms)
	IWDG_SetReload(1562);    //喂狗时间1562*6.4ms=10s,数值不能大于0xfff
	IWDG_ReloadCounter();   //喂狗
	IWDG_Enable();    //使能
}

