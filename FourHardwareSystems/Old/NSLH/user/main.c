#include "stm32_config.h"
#include "modbus.h"
#include "stm32f10x_iwdg.h"

u32 data1=0;
//printf��ӡ��������
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
void normal(void);
	
float dataTemp[12];
float RTDtemperature[4];
float datavalue[8]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0};
float AI_Input[12]={0};


//����ȫ�ֱ���
u8 RS485_RX_BUFF[128];               //���ջ�����2048�ֽ�
u16 RS485_RX_CNT=0;                   //���ռ�����
u8 RS485_FrameFlag=0;                 //֡�������
u8 RS485_TX_BUFF[128];               //���ͻ�����2048�ֽ�
u16 RS485_TX_CNT=0;                   //���ͼ�����

//Modbus�Ĵ����͵�Ƭ���Ĵ���ӳ���ϵ
vu32 *Modbus_InputIO[100];            //���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
vu32 *Modbus_OutputIO[100];           //����������Ĵ���ָ��(����ʹ�õ���λ������)
u16  *Modbus_HoldReg[128];            //���ּĴ���
u8 DataFlag=0;
int main(void)
{
 	RCC_Configuration();            //ϵͳʱ��������Ϣ
	IO_Configuration();             //DI and DO ��ʼ��
	ADC_init();                     //AI��ʼ��
	DAC_init();                     //AO��ʼ��
	RTD_init();                     //���³�ʼ��
	Usart_Total_Initi();            //���ڳ�ʼ��
	Timer1Config();                 //��ʱ���������ж˿�����
	Tim2Configure();               	//��ʱ������С��ͨ������
	Tim4Configure();               	//��ʱ��4��ʱ15s����ʱ����PID control algorithm,ʱ������15s
	
	InitializeTimer(2,2);          	//��ʼ����ʱ��3����ʱ2s����ʱ����˿�����
	
	InitialSys();                   //ϵͳ��ʼ������Ҫ�ǽ��ж˿ڳ�ʼ��
	IWDG_Configuration();           //���Ź���ʼ������
	
	while(1)
	{
		IWDG_ReloadCounter();         //ι��  10s
		ValueTran();                  //���ֲ���ת������Ҫ�Ǵ�����Modbusͨ�ŵ�����
		Modbus_RegMap();              //modbusͨ�����ݣ���ַ���ݴ���
		RS485_Service();              //modbusͨ�Ŵ���

		//14��������ȫ�������ִ�������¶ȿ����߼�
		if(ValveCmd!=0&&ForceValveFlag==0)
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//��ptb����
			APortOut(PTBFan2,8.0);		//��ptb����
			ForceValveFlag=1;
		}
		else if(ValveCmd==0&&ForceValveFlag==1)
		{
			APortOut(CHWV,0.0);
			APortOut(PTBFan1,0.0);		//��ptb����
			APortOut(PTBFan2,0.0);		//��ptb����
			ForceValveFlag=0;
		}
		else if(ValveCmd!=0&&ForceValveFlag==1)
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//��ptb����
			APortOut(PTBFan2,8.0);		//��ptb����
			ForceValveFlag=1;
		}
		
		//DPortOut(ERVFan,1);
		
		//����ERVǿ������������ܵ�ǰERV����ʲô״̬������Ҫ����ERV
		if(ERVCmd!=0)
		{
			DPortOut(ERVFan,1);
			ERVState=1;
			ERVFlag=1;
		}
		if(ERVCmd==0&&ERVFlag==1)   //��ERVǿ��������ERVCmd����Ϊ0�����ر�ERVͨ��
		{
			DPortOut(ERVFan,0);
			ERVState=0;
			ERVFlag=0;
		}
		
		//û��ERVǿ�����ͬʱ���ڷ�ǿ��ģʽ������CO2Ũ�Ƚ��п���
		if(ERVCmd==0&&ERVFlag==0)
		{
			CO2Control();//�ǽ������������У�CO2�����߼
		}
		
		normal();	
	}
}	

//normal logic
void normal(void)
{
			//RunStatusΪϵͳ����״̬��־λ��RunStatus--0:ϵͳδ���У�RunStatus--1��ϵͳ��������
		if(!RunStatus&&(IBSCommand||ScheduleComm||PDVComm))      //�յ�Booking����Schedule��������ϵͳ
		{
			//ϵͳ���������У��ж��Ƿ�ǿ�ƿ���
			if(ValveCmd!=0)
			{
				if(ValveCmd>100)
					ValveCmd=100;
				if(ValveCmd<1)
					ValveCmd=0;
				APortOut(CHWV,ValveCmd/10.0);
				APortOut(PTBFan1,8.0);		//��ptb����
				APortOut(PTBFan2,8.0);		//��ptb����
				ForceValveFlag=1;
				return;
			}
			else if(ValveCmd==0&&ForceValveFlag==1)
			{
				APortOut(CHWV,0.0);
				APortOut(PTBFan1,0.0);		//��ptb����
				APortOut(PTBFan2,0.0);		//��ptb����
				ForceValveFlag=0;
			}
			else if(ValveCmd==0&&ForceValveFlag==0)
			{
				AlarmFlag=NSLH_Start();              					//ϵͳ������ˮ����40%
			}
			
			if(AlarmFlag!=0)                              //���ϵͳ�����쳣���ر����ж˿�����ź�
				NSLH_Clear();
			RunStatus=1;
		}

		//ϵͳ�������й����У��յ�ǿ�ƿ�������
		if(RunStatus&&(ValveCmd!=0))
		{
			if(ValveCmd>100)
				ValveCmd=100;
			if(ValveCmd<1)
				ValveCmd=0;
			APortOut(CHWV,ValveCmd/10.0);
			APortOut(PTBFan1,8.0);		//��ptb����
			APortOut(PTBFan2,8.0);		//��ptb����
			ForceValveFlag=1;
		}
		
		//ϵͳ����ʱ���յ�E-booking����Schedule��ϵͳָ��ʱ���ر�ϵͳ
		if(RunStatus&&((!IBSCommand)&&(!ScheduleComm)&&(!PDVComm)))
		{
			if(ForceValveFlag==1&&(ValveCmd!=0))
			{
				if(ValveCmd>100)
					ValveCmd=100;
				if(ValveCmd<1)
					ValveCmd=0;
				APortOut(CHWV,ValveCmd/10.0);
				APortOut(PTBFan1,8.0);		//��ptb����
				APortOut(PTBFan2,8.0);		//��ptb����
				ForceValveFlag=1;
				return;
			}
			else if(ValveCmd==0&&ForceValveFlag==1)
			{
				APortOut(CHWV,0.0);
				APortOut(PTBFan1,0.0);		//��ptb����
				APortOut(PTBFan2,0.0);		//��ptb����
				ForceValveFlag=0;
			}
			else if(ValveCmd==0&&ForceValveFlag==0)
			{
				AlarmFlag=NSLH_Stop();                      //�ر�ϵͳ
			}	
			if(AlarmFlag!=0)
			{
				NSLH_Clear();                             //���ϵͳ�쳣���ر����ж˿�
			}
			RunStatus=0;
		}
		
		//��ϵͳ��������״̬�£�Motion Sensor��⵽���˽��뷿��
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
				AlarmFlag=OccupanyMode();     //ִ����������¿��Ʋ���
				if(AlarmFlag!=0)              //���ϵͳ�쳣����ر�ȫ���˿���Ϣ
				{
					NSLH_Clear();
				}	
			}
		}
		
		//��ϵͳ��������״̬�£�Motion Sensor��⵽��������״̬��
		if(RunStatus&&(!OccSta)&&ForceValveFlag==0)
		{
			RoomTempSv=250;              //����״���£������¶��趨ֵΪ27�ȣ����ֲ���
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
				AlarmFlag=Unoccupanied();    //ִ������ģʽ�£��¶ȿ��Ʋ������¶��趨ֵΪ27�ȣ�PID����ˮ��
				if(AlarmFlag!=0)
				{
					NSLH_Clear();
				}
			}
		}
}


void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                              //�����ⲿ���پ�������״̬ö�ٱ�����
	RCC_DeInit();                                              //��λRCC�Ĵ�����Ĭ��ֵ��
	RCC_HSEConfig(RCC_HSE_ON);                                 //���ⲿ�ĸ��پ��� 
	HSEStartUpStatus = RCC_WaitForHSEStartUp();                //�ȴ��ⲿ����ʱ��׼����
	if(HSEStartUpStatus == SUCCESS)
		{
			RCC_HCLKConfig(RCC_SYSCLK_Div1);                         //����AHB
			RCC_PCLK2Config(RCC_HCLK_Div1);                          //����APB2 
			RCC_PCLK1Config(RCC_HCLK_Div2);                          //����APB1 
			//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	      
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //PLL=8M*9=72M
			RCC_PLLCmd(ENABLE);                                      //ʱ��PLL
			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);      //�ȴ�PLL����	      
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);               //����ϵͳʱ��=PLL	  
			while(RCC_GetSYSCLKSource() != 0x08);                    //���PLLʱ���Ƿ���Ϊϵͳʱ��	       
		}
}

PUTCHAR_PROTOTYPE    //ʹ��printf����ʱ�����Խ�����ͨ�����ڴ�ӡ����
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
	
	//С����
	OccSta=DI_data[0]*10;
	
	//�󷿼䣬����101
	//OccSta=(DI_data[0]|DI_data[1])*10;
	
	//�������� ���ŷ���ֵ������δ���
	ValveFb=(AI_Input[CHWVfb]*10+0.5);
	
	//��������ΪNNSLH1L106,���ŷ���ʹ�øöδ��룬ͬʱ���ε��϶δ���
//	if(ValveCmd!=0)
//	{
//		ValveFb=ValveCmd;
//	}
//	else if(ValveCmd==0)
//	{
//		ValveFb=ValvePosition*10;
//	}
	
	DPValue=(AI_Input[3]*10);
	
	//��ȡ���ŷ���
	APortIn(CHWVfb,AI_Input);
	//��ȡѹ���
	APortIn(3,AI_Input);
}

void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //д��0x5555����������Ĵ���д�빦�ܣ�����Ĵ�������
	IWDG_SetPrescaler(IWDG_Prescaler_256);     //ʱ�ӷ�Ƶ 40K/256=156HZ(6.4ms)
	IWDG_SetReload(1562);    //ι��ʱ��1562*6.4ms=10s,��ֵ���ܴ���0xfff
	IWDG_ReloadCounter();   //ι��
	IWDG_Enable();    //ʹ��
}

