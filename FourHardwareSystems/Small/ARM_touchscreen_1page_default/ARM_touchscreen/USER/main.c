#include "stm32f10x_config.h"
#include "UI.h"
#include "touch.h"
#include "stm32f10_timer.h"
#include "LCD.h"
#include "stm32f10_gpio.h"
#include "stm32f10_usart.h" 
#include "math.h"	  
#include "stm32f10x_iwdg.h"
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
/*******************************************************************************
  * @brief  Main program.
  * @param  None
  * @retval None
  ******************************************************************************/
u8  trahdatabuffer[60][4];
long int  trahdatasum[4];
u8  trahdataave[4];

u16 PvRH=600,PvTe=255,SvTe=0,PvCO2=0,SvCO2=0,PSvTe=250, PSVCO2value=500;
u8  Tflag=0,Oflag=0,touchcase=0;
u8  Startflag=0,Confirmflag=0;

volatile  u8 PAGEflag=3;
char ss[6];

unsigned char getdataflag=0,lcdBG=1,lcdBG_timecount=0,onoff_flag=0,clearflag=0,send_flag=0;
unsigned char timcount=0;
GPIO_InitTypeDef GPIO_InitStruct;

int PCO2value=0;
float PCO2valueO=0;
float IC1valueav=0; 
float IC2valueav=0; 

int IC1sumvaluesum;
int IC2sumvaluesum;
int IC1sum[20]={0};
int IC2sum[20]={0};
u8 cnti=0;
u8 i=0;
u8 pageFinish=0;
extern u8 co2flag;

int main(void)
{
	
    RCC_Configuration(); //系统时钟配置
    NVIC_Configuration();
	//SPI_Flash_Init();//初始化Flash;
    Usart_Configuration(); //串口配置
    Time_Configuration();  //定时器配置
	IWDG_Configuration();
	Usart2ST(); //开启串口2中断
    Tim3ST(); //开启时钟3
    Tim1ST();
	Tim2ST();
    Tim4ST() ;
	Tim5ST() ;

	LCD_Init();
    TP_Init();
	LCD_Clear(WHITE);
	PAGE0();
	PAGE1(PvRH,PvTe,PSvTe,PCO2value);


	while(1)
	{
		IWDG_ReloadCounter(); 
		if (getdataflag!=0)
		{
			getdataflag=0;
			timcount++;
			switch(send_flag)
			{
				case 0:if (timcount>=5){usart2_senddata(0);timcount=0;}break; //??5??
				case 1: usart2_senddata(1);send_flag=0;break;
                case 2: usart2_senddata(2);send_flag=0;break;
                case 3: usart2_senddata(3);send_flag=0;break;	

				case 4: usart2_senddata(4);send_flag=0;break;
                case 5: usart2_senddata(5);send_flag=0;break;	//start

				default:break;
			}

			 for(cnti=0;cnti<20;cnti++)
			{
			   IC1sumvaluesum+=IC1sum[cnti]	;
			   IC2sumvaluesum+=IC2sum[cnti]	;
			 }
			 IC1valueav =	 IC1sumvaluesum/20;
			 IC2valueav =	 IC2sumvaluesum/20;
			
//			 PCO2valueO = ((IC1valueav/IC2valueav)*1004-2)*2;
//			 PCO2value =rint(2.34*(PCO2valueO)-176.32);
			 
			 PCO2valueO = 2000.0*(IC1valueav-20)/(IC2valueav-40); 
//			 PCO2value = rint(1.8529*(PCO2valueO)+340);
			 if  (PCO2valueO>=266.0)
			 	PCO2value   = rint(0.90*(PCO2valueO)+340)   ;
			 else
			  PCO2value =rint(2.10*(PCO2valueO)+20.4) ;
			 if(PCO2value<400)
				 PCO2value=400;
			 if(PCO2value>2000)
				 PCO2value=2000;
			 if((IC1sum[0]==0)|(IC2valueav==40))
			 PCO2value =400;
			 IC1sumvaluesum =0;
			 IC2sumvaluesum =0;
		}
			  	 
    	if(	PCin(10)==0)
		{
			lcdBG_timecount=0;
			clearflag=1;
			if (lcdBG==0)
			{
			    PCout(6)=1; 	
				lcdBG=1;
			}

			if(Oflag==2)
			{
				touchcase=function();
				Oflag=10;
			}
			
			else if(Oflag==0)
			{
				Oflag=2;
			//	touchcase=function();
              //  timer 
			}
			else if(Oflag==1)
			{
		  	//	pageFinish=0;	
//			  if((touchcase==8)&(PAGEflag==3))
//		  	{	 
//			    send_flag=4;
//		     	pageFinish = PAGE1(PvRH,PvTe,PCO2value);PAGEflag=1;
//				Startflag=1;
//
//			}
//
//			  if(Startflag)
//			  {
//				switch(touchcase)
//				{
//					case 1:if (onoff_flag==0)
//					{
//						send_flag=2;
//						Confirmflag=0;
//					}
//					break;
//					case 2:
//					if (PAGEflag==1)
//					{
//					  Tim4SP();
//					  pageFinish = PAGE2(PSvTe,PSVCO2value);
//					  PAGEflag=2;
//					  Confirmflag=0;
//					  Tim4ST();
//					} 			
//					else if(PAGEflag==2)
//					{
//					  Tim4SP();
//					  pageFinish =PAGE1(PvRH,PvTe,PCO2value);
//					  PAGEflag=1;
//					  Confirmflag=0;
//					  Tim4ST();
//					}
//					break;
//					case 3:
//				    if  (PAGEflag==2)
//					{	Tim4SP();
//						pageFinish = PAGE1(PvRH,PvTe,PCO2value);PAGEflag=1;
//						packagedata(PvRH,PvTe,PSvTe,PCO2value,PSVCO2value,Startflag);
//						send_flag=1;
//						Confirmflag=0;
//						 Tim4ST();
//					}
//					break;
//					case 4: 
//
//					if ((PAGEflag==1)|(PAGEflag==2))
//					{
//
//					   if (onoff_flag==1)
//					 	{
//						 send_flag=3;
//					 	}
//						Confirmflag=0;
//
//					} 			
//					else if(PAGEflag==3)
//					{
//					  Tim4SP();
//					  pageFinish =PAGE1(PvRH,PvTe,PCO2value);
//					  PAGEflag=1;
//					  Confirmflag=0;
//					  Tim4ST();
//					}
//					break;
//
//
//					case 5:
//
//				    if  ((PAGEflag==1)|(PAGEflag==2))
//					{	
//					    Tim4SP();
//						pageFinish = PAGE3();PAGEflag=3;
//						Confirmflag=0;
//						Tim4ST();
//					}
//				
//					break;
//
//				   	case 9:
//				    if  (PAGEflag==3)
//					{ 
//					
//					  //pageFinish = PAGE1(PvRH,PvTe,PCO2value);PAGEflag=1;
//					    Confirmflag++;
//						if(Confirmflag==1) 
//						 	showzifustr(290,60,"Confirm Stop",0xffff,0x0000,1);
//						if(Confirmflag==2)
//					    { 	send_flag=5; 
//							Confirmflag=0;
//							delay_ms(100); 
//							Startflag=0;
//							pageFinish = PAGE3();PAGEflag=3;	 
//						}
//					
//					
//					}
//					break;
//      				case 10:if ((PAGEflag==2)&&(PSvTe>0))
//					{
//						PSvTe--;
//						inttostrA(PSvTe,ss);
//                     	showzifustr(70,120,ss,0,BLUEBG,2);
//						Confirmflag=0;
//					}
//					break;
//					case 11:if (PAGEflag==2)
//					{
//						PSvTe++;
//						inttostrA(PSvTe,ss);
//						showzifustr(70,120,ss,0,BLUEBG,2);
//						Confirmflag=0;
//
//					}break;
//					case 12:if (PAGEflag==2)
//					{
//						PSVCO2value--;
//						if(PSVCO2value<500)
//							PSVCO2value=500;
//					    if(PSVCO2value==999)
//						   LCD_Fill(290,100,435,180,BLUEBG);
//						inttostr(PSVCO2value,ss);
//						showzifustr(312,120,ss,0,BLUEBG,2);
//
//						Confirmflag=0;
//					}break;
//					case 13:if (PAGEflag==2)
//					{
//						PSVCO2value++;
//						if(PSVCO2value>1000)
//							PSVCO2value=1000;
//						inttostr(PSVCO2value,ss);
//
//						showzifustr(312,120,ss,0,BLUEBG,2);
//						Confirmflag=0;
//					}break;
//
//					default:break;
//				}
//			  }
//				touchcase=0;
			}
		}
		else if((PCin(10)!=0)&&(clearflag==1))
		{
			touchcase=0;
			touch_clear();
		}
	}
}
 
/*******************************************************************************
  * @brief  System clock setting.
  * @param  None
  * @retval None
  ******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus; //定义外部高速晶体启动状态枚举变量；
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_DeInit();//复位RCC寄存器到默认值；
	RCC_HSEConfig(RCC_HSE_ON);//打开外部的高速晶振； 
	HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待外部高速时钟准备好

	if(HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//配置AHB
		RCC_PCLK2Config(RCC_HCLK_Div1);//配置APB2 
		RCC_PCLK1Config(RCC_HCLK_Div2);//配置APB1 
		//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	      
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//PLL=8M*9=72M
		RCC_PLLCmd(ENABLE);//时能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//等待PLL就绪	      
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);// 配置系统时钟=PLL	  
		while(RCC_GetSYSCLKSource() != 0x08);//检查PLL时钟是否作为系统时钟	       
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //PA.9
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStruct); 
    PCout(6)=1;

}

void dataProcess()
{
    int Mtemp =0 ;
    int Mhum  =0 ;
	
	ReadDHT11();

//	Mtemp  = (tdata[2]*256+tdata[3])-28;
    Mtemp  = (tdata[2]*256+tdata[3]);
	Mhum   = (tdata[0]*256+tdata[1]);
	
	if((Mtemp>320)|(Mtemp<180))
	 Mtemp = 255 ;

	inttostrA(Mtemp,ss);
	showzifustr(70,110,ss,0,BLUEBG,2);
	inttostrA(PSvTe,ss);
	showzifustr(70,200,ss,0,BLUEBG,1);

	inttostr(PCO2value,ss);
	showzifustr(302,188,ss,0,BLUEBG,1);
	inttostrA(Mhum,ss);
	showzifustr(302,90,ss,0,BLUEBG,1);
	
	PvRH = Mhum   ;
	PvTe =	Mtemp  ;
 	packagedata(PvRH,PvTe,PSvTe,PCO2value,PSVCO2value,Startflag);
}

void Send_finish(unsigned char a)
{
	switch(a)
	{
		case 1:
			showonoff(1);
		break;
		
		case 2:
			showonoff(0);
		break;
		
		case 3:
//			PAGE1(PvRH,PvTe,PCO2value);
//			PAGEflag=1;
		break;
		case 4:
			Startflag=1;
		break;
		case 5:
	        Startflag=0;
		default:break;
	}
}

void showonoff(int onoff)
{
	if(onoff==1)
	{
		onoff_flag=1;
		showzifustr(10,247,"ON",0x5ACB,0X01CF,1);
		showzifustr(410,247,"OFF",0xFFFF,0X01CF,1);

	}
	else if(onoff==0)
	{
		onoff_flag=0;
		showzifustr(10,247,"ON",0xFFFF,0X01CF,1);
		showzifustr(410,247,"OFF",0x5ACB,0X01CF,1);
	}	

}





PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(u8)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return ch;
}

void IWDG_Configuration(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);      
    IWDG_SetPrescaler(IWDG_Prescaler_256);      //40K/256=156HZ(6.4ms)
    IWDG_SetReload(1562);       
    IWDG_ReloadCounter();
    IWDG_Enable();
}
