#include "stm32_config.h"
#include "modbus.h"
#include "stm32f10x_iwdg.h"

u32 data1=0;
//printf´òÓ¡µ½´®¿ÚÖÐ
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
void normal(void);
	
float dataTemp[12];
float RTDtemperature[4];
float datavalue[8]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0};
float AI_Input[12]={0};


//¶¨ÒåÈ«¾Ö±äÁ¿
u8 RS485_RX_BUFF[128];               //½ÓÊÕ»º´æÇø2048×Ö½Ú
u16 RS485_RX_CNT=0;                   //½ÓÊÕ¼ÆÊýÆ÷
u8 RS485_FrameFlag=0;                 //Ö¡½áÊø±ê¼Ç
u8 RS485_TX_BUFF[128];               //·¢ËÍ»º´æÇø2048×Ö½Ú
u16 RS485_TX_CNT=0;                   //·¢ËÍ¼ÆÊýÆ÷

//Modbus¼Ä´æÆ÷ºÍµ¥Æ¬»ú¼Ä´æÆ÷Ó³Éä¹ØÏµ
vu32 *Modbus_InputIO[100];            //ÊäÈë¿ª¹ØÁ¿¼Ä´æÆ÷Ö¸Õë(ÕâÀïÊ¹ÓÃµÄÊÇÎ»´ø²Ù×÷)
vu32 *Modbus_OutputIO[100];           //Êä³ö¿ª¹ØÁ¿¼Ä´æÆ÷Ö¸Õë(ÕâÀïÊ¹ÓÃµÄÊÇÎ»´ø²Ù×÷)
u16  *Modbus_HoldReg[128];            //±£³Ö¼Ä´æÆ÷
u8 DataFlag=0;
int main(void)
{
 	RCC_Configuration();            //ÏµÍ³Ê±ÖÓÅäÖÃÐÅÏ¢
	IO_Configuration();             //DI and DO ³õÊ¼»¯
	ADC_init();                     //AI³õÊ¼»¯
	DAC_init();                     //AO³õÊ¼»¯
	RTD_init();                     //²âÎÂ³õÊ¼»¯
	Usart_Total_Initi();            //´®¿Ú³õÊ¼»¯
	Timer1Config();                 //¶¨Ê±´¦Àí´ó°åËùÓÐ¶Ë¿ÚÊý¾Ý
	Tim2Configure();               	//¶¨Ê±´¦ÀíÓëÐ¡°åÍ¨ÐÅÊý¾Ý
	Tim4Configure();               	//¶¨Ê±Æ÷4¶¨Ê±15s£¬¶¨Ê±´¦ÀíPID control algorithm,Ê±¼ä¼ä¸ôÊÇ15s
	
	InitializeTimer(2,2);          	//³õÊ¼»¯¶¨Ê±Æ÷3£¬¶¨Ê±2s£¬¶¨Ê±´¦Àí¶Ë¿ÚÊý¾Ý
	
	InitialSys();                   //ÏµÍ³³õÊ¼»¯£¬Ö÷ÒªÊÇ½øÐÐ¶Ë¿Ú³õÊ¼»¯
	IWDG_Configuration();           //¿´ÃÅ¹·³õÊ¼»¯ÅäÖÃ
	
	while(1)
	{
		IWDG_ReloadCounter();         //Î¹¹·  10s
		ValueTran();                  //²¿·Ö²ÎÊý×ª»»£¬Ö÷ÒªÊÇ´¦ÀíÓëModbusÍ¨ÐÅµÄÊý¾Ý
		Modbus_RegMap();              //modbusÍ¨ÐÅÊý¾Ý£¬µØÖ·Êý¾Ý´¦Àí
		RS485_Service();              //modbusÍ¨ÐÅ´¦Àí

		//14£º·§½ô¼±È«¿ªÃüÁî£¬²»Ö´ÐÐÕý³£ÎÂ¶È¿ØÖÆÂß¼­
		if(ValveCmd!=0&&ForceValveFlag==0)
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//¿ªptb·çÉÈ
			APortOut(PTBFan2,8.0);		//¿ªptb·çÉÈ
			ForceValveFlag=1;
		}
		else if(ValveCmd==0&&ForceValveFlag==1)
		{
			APortOut(CHWV,0.0);
			APortOut(PTBFan1,0.0);		//¿ªptb·çÉÈ
			APortOut(PTBFan2,0.0);		//¿ªptb·çÉÈ
			ForceValveFlag=0;
		}
		else if(ValveCmd!=0&&ForceValveFlag==1)
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//¿ªptb·çÉÈ
			APortOut(PTBFan2,8.0);		//¿ªptb·çÉÈ
			ForceValveFlag=1;
		}
		
		//DPortOut(ERVFan,1);
		
		//ÓÐÁËERVÇ¿ÖÆÆô¶¯ÃüÁî£¬²»¹Üµ±Ç°ERV´¦ÓÚÊ²Ã´×´Ì¬£¬¶¼ÐèÒª¿ªÆôERV
		if(ERVCmd!=0)
		{
			DPortOut(ERVFan,1);
			ERVState=1;
			ERVFlag=1;
		}
		if(ERVCmd==0&&ERVFlag==1)   //ÔÚERVÇ¿ÖÆÆô¶¯ºó£¬ERVCmdÃüÁîÎª0£¬¼´¹Ø±ÕERVÍ¨·ç
		{
			DPortOut(ERVFan,0);
			ERVState=0;
			ERVFlag=0;
		}
		
		//Ã»ÓÐERVÇ¿ÖÆÃüÁî£¬Í¬Ê±´¦ÓÚ·ÇÇ¿ÖÆÄ£Ê½£¬¸ù¾ÝCO2Å¨¶È½øÐÐ¿ØÖÆ
		if(ERVCmd==0&&ERVFlag==0)
		{
			CO2Control();//·Ç½ô¼±µÄÕý³£ÔËÐÐ£¬CO2¿ØÖÆÂß¼
		}
		
		normal();	
	}
}	

//normal logic
void normal(void)
{
			//RunStatusÎªÏµÍ³ÔËÐÐ×´Ì¬±êÖ¾Î»£¬RunStatus--0:ÏµÍ³Î´ÔËÐÐ£¬RunStatus--1£ºÏµÍ³Õý³£ÔËÐÐ
		if(!RunStatus&&(IBSCommand||ScheduleComm||PDVComm))      //ÊÕµ½Booking»òÕßScheduleÃüÁîÆô¶¯ÏµÍ³
		{
			//ÏµÍ³Æô¶¯¹ý³ÌÖÐ£¬ÅÐ¶ÏÊÇ·ñÇ¿ÖÆ¿ª·§
			if(ValveCmd!=0)
			{
				if(ValveCmd>100)
					ValveCmd=100;
				if(ValveCmd<1)
					ValveCmd=0;
				APortOut(CHWV,ValveCmd/10.0);
				APortOut(PTBFan1,8.0);		//¿ªptb·çÉÈ
				APortOut(PTBFan2,8.0);		//¿ªptb·çÉÈ
				ForceValveFlag=1;
				return;
			}
			else if(ValveCmd==0&&ForceValveFlag==1)
			{
				APortOut(CHWV,0.0);
				APortOut(PTBFan1,0.0);		//¿ªptb·çÉÈ
				APortOut(PTBFan2,0.0);		//¿ªptb·çÉÈ
				ForceValveFlag=0;
			}
			else if(ValveCmd==0&&ForceValveFlag==0)
			{
				AlarmFlag=NSLH_Start();              					//ÏµÍ³Æô¶¯£¬Ë®·§¿ª40%
			}
			
			if(AlarmFlag!=0)                              //Èç¹ûÏµÍ³Æô¶¯Òì³££¬¹Ø±ÕËùÓÐ¶Ë¿ÚÊä³öÐÅºÅ
				NSLH_Clear();
			RunStatus=1;
		}

		//ÏµÍ³ÕýÔÚÔËÐÐ¹ý³ÌÖÐ£¬ÊÕµ½Ç¿ÖÆ¿ª·§ÃüÁî
		if(RunStatus&&(ValveCmd!=0))
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//¿ªptb·çÉÈ
			APortOut(PTBFan2,8.0);		//¿ªptb·çÉÈ
			ForceValveFlag=1;
		}
		
		//ÏµÍ³ÔËÐÐÊ±£¬ÊÕµ½E-booking»òÕßSchedule¹ØÏµÍ³Ö¸ÁîÊ±£¬¹Ø±ÕÏµÍ³
		if(RunStatus&&((!IBSCommand)&&(!ScheduleComm)&&(!PDVComm)))
		{
			if(ForceValveFlag==1&&(ValveCmd!=0))
			{
				if(ValveCmd>100)
					ValveCmd=100;
				if(ValveCmd<1)
					ValveCmd=0;
				APortOut(CHWV,ValveCmd/10.0);
				APortOut(PTBFan1,8.0);		//¿ªptb·çÉÈ
				APortOut(PTBFan2,8.0);		//¿ªptb·çÉÈ
				ForceValveFlag=1;
				return;
			}
			else if(ValveCmd==0&&ForceValveFlag==1)
			{
				APortOut(CHWV,0.0);
				APortOut(PTBFan1,0.0);		//¹Øptb·çÉÈ
				APortOut(PTBFan2,0.0);		//¹Øptb·çÉÈ
				ForceValveFlag=0;
			}
			else if(ValveCmd==0&&ForceValveFlag==0)
			{
				AlarmFlag=NSLH_Stop();                      //¹Ø±ÕÏµÍ³
			}	
			if(AlarmFlag!=0)
			{
				NSLH_Clear();                             //Èç¹ûÏµÍ³Òì³££¬¹Ø±ÕËùÓÐ¶Ë¿Ú
			}
			RunStatus=0;
		}
		
		//ÔÚÏµÍ³Õý³£ÔËÐÐ×´Ì¬ÏÂ£¬Motion Sensor¼ì²âµ½ÓÐÈË½øÈë·¿¼ä
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
				AlarmFlag=OccupanyMode();     //Ö´ÐÐÓÐÈËÇé¿öÏÂ¿ØÖÆ²Ù×÷
				if(AlarmFlag!=0)              //Èç¹ûÏµÍ³Òì³££¬Ôò¹Ø±ÕÈ«²¿¶Ë¿ÚÐÅÏ¢
				{
					NSLH_Clear();
				}	
			}
		}
		
		//ÔÚÏµÍ³Õý³£ÔËÐÐ×´Ì¬ÏÂ£¬Motion Sensor¼ì²âµ½·¿¼äÎÞÈË×´Ì¬ÏÂ
		if(RunStatus&&(!OccSta)&&ForceValveFlag==0)
		{
			RoomTempSv=250;              //ÎÞÈË×´¿öÏÂ£¬·¿¼äÎÂ¶ÈÉè¶¨ÖµÎª27¶È£¬±£³Ö²»±ä
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
				AlarmFlag=Unoccupanied();    //Ö´ÐÐÎÞÈËÄ£Ê½ÏÂ£¬ÎÂ¶È¿ØÖÆ²Ù×÷£¬ÎÂ¶ÈÉè¶¨ÖµÎª27¶È£¬PIDµ÷½ÚË®·§
				if(AlarmFlag!=0)
				{
					NSLH_Clear();
				}
			}
		}
}


void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                              //¶¨ÒåÍâ²¿¸ßËÙ¾§ÌåÆô¶¯×´Ì¬Ã¶¾Ù±äÁ¿£»
	RCC_DeInit();                                              //¸´Î»RCC¼Ä´æÆ÷µ½Ä¬ÈÏÖµ£»
	RCC_HSEConfig(RCC_HSE_ON);                                 //´ò¿ªÍâ²¿µÄ¸ßËÙ¾§Õñ£» 
	HSEStartUpStatus = RCC_WaitForHSEStartUp();                //µÈ´ýÍâ²¿¸ßËÙÊ±ÖÓ×¼±¸ºÃ
	if(HSEStartUpStatus == SUCCESS)
		{
			RCC_HCLKConfig(RCC_SYSCLK_Div1);                         //ÅäÖÃAHB
			RCC_PCLK2Config(RCC_HCLK_Div1);                          //ÅäÖÃAPB2 
			RCC_PCLK1Config(RCC_HCLK_Div2);                          //ÅäÖÃAPB1 
			//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	      
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //PLL=8M*9=72M
			RCC_PLLCmd(ENABLE);                                      //Ê±ÄÜPLL
			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);      //µÈ´ýPLL¾ÍÐ÷	      
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);               //ÅäÖÃÏµÍ³Ê±ÖÓ=PLL	  
			while(RCC_GetSYSCLKSource() != 0x08);                    //¼ì²éPLLÊ±ÖÓÊÇ·ñ×÷ÎªÏµÍ³Ê±ÖÓ	       
		}
}

PUTCHAR_PROTOTYPE    //Ê¹ÓÃprintfº¯ÊýÊ±£¬¿ÉÒÔ½«Êý¾ÝÍ¨¹ý´®¿Ú´òÓ¡³öÀ´
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
	
	//Ð¡·¿¼ä
	OccSta=DI_data[0]*10;
	
	//´ó·¿¼ä£¬Á½¸ö101
	//OccSta=(DI_data[0]|DI_data[1])*10;
	
	//ÆäËû·¿¼ä ·§ÃÅ·´À¡Öµ²ÅÓÐÕâ¶Î´úÂë
	ValveFb=(AI_Input[CHWVfb]*10+0.5);
	
	//Èç¹û·¿¼äºÅÎªNNSLH1L106,·§ÃÅ·´À¡Ê¹ÓÃ¸Ã¶Î´úÂë£¬Í¬Ê±ÆÁ±ÎµôÉÏ¶Î´úÂë
//	if(ValveCmd!=0)
//	{
//		ValveFb=ValveCmd;
//	}
//	else if(ValveCmd==0)
//	{
//		ValveFb=ValvePosition*10;
//	}
	
	DPValue=(AI_Input[3]*10);
	
	//¶ÁÈ¡·§ÃÅ·´À¡
	APortIn(CHWVfb,AI_Input);
	//¶ÁÈ¡Ñ¹²î·´À¡
	APortIn(3,AI_Input);
}

void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //Ð´Èë0x5555£¬ÓÃÓÚÔÊÐí¼Ä´æÆ÷Ð´Èë¹¦ÄÜ£¬½â³ý¼Ä´æÆ÷±£»¤
	IWDG_SetPrescaler(IWDG_Prescaler_256);     //Ê±ÖÓ·ÖÆµ 40K/256=156HZ(6.4ms)
	IWDG_SetReload(1562);    //Î¹¹·Ê±¼ä1562*6.4ms=10s,ÊýÖµ²»ÄÜ´óÓÚ0xfff
	IWDG_ReloadCounter();   //Î¹¹·
	IWDG_Enable();    //Ê¹ÄÜ
}

