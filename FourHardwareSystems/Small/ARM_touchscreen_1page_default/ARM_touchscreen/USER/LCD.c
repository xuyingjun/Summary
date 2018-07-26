#include"LCD.h"
#include "ziku.h"
#include "zikularge.h"
#include "stdlib.h"
#include "stm32f10x_fsmc.h"


void LCD_Writ_Bus(char VH,char VL) 
{
//	DATAH=VH;
//	DATAL=VL;
//	LCD_WR_L;
//	LCD_WR_H;
}

void LCD_WR_DATA(u16 data)
{
	 LCD->LCD_RAM=data;//16位接口
		
}

void LCD_WR_REG(u16 regval)
{
    LCD->LCD_REG=regval;
}

void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}
////读LCD数据
////返回值:读到的值
//u16 LCD_RD_DATA(void)
//{										    	   
//	return LCD->LCD_RAM;	
//
//}
//u16 LCD_ReadReg(u8 LCD_Reg)
//{
//	u16 myid=0;
//	u8 x=0;
//	while(x<5 && (myid==0||myid==0xffff))	  //读5次
//	{	
//		LCD_WR_REG_DATA(0x0000,0x0001);										   
//		delay_us(200);
//		LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
//		delay_us(5);		  
//		myid=LCD_RD_DATA();		//返回读到的值
//		x++;
//	}
//	return myid;
//} 


void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	LCD_WR_REG(0x002A);
	LCD_WR_DATA(x1>>8);
	LCD_WR_DATA(x1&0x00ff);
	LCD_WR_DATA(x2>>8);
	LCD_WR_DATA(x2&0x00ff);
    LCD_WR_REG(0x002b);
	LCD_WR_DATA(y1>>8);
	LCD_WR_DATA(y1&0x00ff);
	LCD_WR_DATA(y2>>8);
	LCD_WR_DATA(y2&0x00ff);
	LCD_WR_REG(0x002c);
}


void LCD_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

   	// Enable FSMC, GPIOD, GPIOE clocks
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE,ENABLE);

	// Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.07(NE1), PD.08(D13),
  	// PD.09(D14), PD.10(D15), PD.13(A18), PD.14(D0), PD.15(D1) as alternate 
  	// function push pull 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| 
                                  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

  	// Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
  	// PE.14(D11), PE.15(D12) as alternate function push pull */
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                   GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                   GPIO_Pin_15;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);

  	//PB14 --> LCD_RST
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	readWriteTiming.FSMC_AddressSetupTime = 2;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 1;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 10;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 1;
    readWriteTiming.FSMC_CLKDivision = 1;
    readWriteTiming.FSMC_DataLatency = 1;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 2;	 //地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 1;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 10;		 ////数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration =1;
    writeTiming.FSMC_CLKDivision =1;
    writeTiming.FSMC_DataLatency =1;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置		

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1 
	
	LCD_REST=0;		 
 	delay_ms(50); // delay 20 ms 
   	LCD_REST=1;		 
 	delay_ms(50); // delay 20 ms 



	LCD_WR_REG(0x00E2);	//PLL multiplier, set PLL clock to 120M
	LCD_WR_DATA(0x002d);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_DATA(0x0002);
	LCD_WR_DATA(0x0004);
	LCD_WR_REG(0x00E0);  // PLL enable
	LCD_WR_DATA(0x0001);
	delay_ms(1);
	LCD_WR_REG(0x00E0);
	LCD_WR_DATA(0x0003);
	delay_ms(5);
	LCD_WR_REG(0x0001);  // software reset
	delay_ms(5);
	LCD_WR_REG(0x00E6);	//PLL setting for PCLK, depends on resolution
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA(0x00ff);
	LCD_WR_DATA(0x00be);

	LCD_WR_REG(0x00B0);	//LCD SPECIFICATION
	LCD_WR_DATA(0x0020);
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_DATA(HDP&0X00FF);
    LCD_WR_DATA((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_DATA(VDP&0X00FF);
    LCD_WR_DATA(0x0000);
	delay_ms(5);
	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_DATA((HT>>8)&0X00FF);  //Set HT
	LCD_WR_DATA(HT&0X00FF);
	LCD_WR_DATA((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_DATA(HPS&0X00FF);
	LCD_WR_DATA(HPW);			   //Set HPW
	LCD_WR_DATA((LPS>>8)&0X00FF);  //SetLPS
	LCD_WR_DATA(LPS&0X00FF);
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_DATA((VT>>8)&0X00FF);   //Set VT
	LCD_WR_DATA(VT&0X00FF);
	LCD_WR_DATA((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_DATA(VPS&0X00FF);
	LCD_WR_DATA(VPW);			   //Set VPW
	LCD_WR_DATA((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_DATA(FPS&0X00FF);


	LCD_WR_REG(0x0036); //rotation
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00F0); //pixel data interface
	LCD_WR_DATA(0x0003);


	delay_ms(5);


	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_DATA(0x0006);
	LCD_WR_DATA(0x00f0);
	LCD_WR_DATA(0x0001);
	LCD_WR_DATA(0x00f0);
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00d0); 
	LCD_WR_DATA(0x000d);

    //----------LCD RESET---GPIO0-------------------//
	LCD_WR_REG(0x00B8);
	LCD_WR_DATA(0x0000);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_DATA(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x00BA);
	LCD_WR_DATA(0x0000);  
//	 delay_ms(50);
//	LCD_Clear(WHITE); 
   }
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2c); 
}
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_REG(0x002A);	
	LCD_WR_DATA(Xpos>>8);	    
	LCD_WR_DATA(Xpos&0x00ff);
	LCD_WR_DATA(479>>8);	    
	LCD_WR_DATA(479&0x00ff);
	LCD_WR_REG(0x002b);	
	LCD_WR_DATA(Ypos>>8);	    
	LCD_WR_DATA(Ypos&0x00ff);
	LCD_WR_DATA(271>>8);	    
	LCD_WR_DATA(271&0x00ff);
	 
}
void LCD_Clear(u16 color)
{


	int Ci=0,Cj=0;
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(Ci=0;Ci<LCD_W;Ci++)
	 {
	  		for (Cj=0;Cj<LCD_H;Cj++)
	   		{
        	 	 LCD_WR_DATA(color);
	    	}
	  }
	
}

void LCD_Main_Clear(unsigned int Color)
{
	int Ci=0,Cj=0;
	Address_set(0,35,479,243);
    for(Ci=0;Ci<479;Ci++)
	 {
	  		for (Cj=0;Cj<243;Cj++)
	   		{
        	 	 LCD_WR_DATA(Color);
	    	}
	  }
}

void LCD_DrawPoint(u16 x,u16 y,u16 data)
{
	LCD_SetCursor(x,y);		//设置光标位置 
//	delay_us(1);
	LCD_WriteRAM_Prepare();	//开始写入GRAM
//	delay_us(1);
	LCD_WR_DATA(data); 	    
}

void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{          
	unsigned int i,j; 
	Address_set(xsta,ysta,xend,yend);      //
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//
	} 					  	    
}


u16 showzifularge(unsigned int x,unsigned int y,char val,unsigned int dcolor,unsigned int bgcolor)
{  
	u16 i,j;
	u8 temp=0;
	u16 x0=x;
	u16 y0=y;
	u16 x1=x;
	u16 y1=y;
    u16 OffsetValue=0;
	u16 largeOffsetend=0;
	u16 lengthValue=0;
	u16 k=0;
	u16 step =0;
	u16 POINT_COLOR=0;					   
	k+=(val-32);
	OffsetValue =paraF[k].offset	;
	largeOffsetend =paraF[k+1].offset	;
	lengthValue	  = paraF[k].length+1;
	step =largeOffsetend-OffsetValue ;

	for(j=0;j<step;j++)
	{
	    temp = zikularge[OffsetValue];
		OffsetValue++;
		for(i=0;i<8;i++)
		{
	   	        if(temp&0x80)
				{POINT_COLOR=dcolor;
				}
				else 
				{POINT_COLOR=bgcolor;
				}
				LCD_DrawPoint(x1,y1, POINT_COLOR);	
				temp<<=1;
				x1++;
				if(x1>=LCD_W)
				{
				 POINT_COLOR=bgcolor;
				 return lengthValue;
				 }//超区域了
				if((x1-x0)==(lengthValue))
				{
					x1=x0;
					y1++;
					if(y1>=LCD_H)
					{POINT_COLOR=bgcolor;return lengthValue ;}//超区域了
					else if(y1-y0>=50)
					return lengthValue;
					else
					break;
				}
					
		}
	
	 }
	 return		lengthValue;

}

void showzifu(unsigned int x,unsigned int y,char val,unsigned int dcolor,unsigned int bgcolor)
{  
	unsigned char i,j;
	unsigned int k=0;
    Address_set(x,y,x+15,y+21); //????
	k+=(val-32)*42;
	for(j=0;j<42;j++)
	{
		for(i=0;i<8;i++)
		{
	    	if((ziku[k]&(1<<(7-i)))!=0)
	
			{
				LCD_WR_DATA(dcolor);
			}
			else
			{
				LCD_WR_DATA(bgcolor);
			}
		}
		k++;
	 }
}

void showzifustr(unsigned int x,unsigned int y,char *str,unsigned int dcolor,unsigned int bgcolor,unsigned char size)	  
{  
	unsigned int x1,y1;
	u16 endValue=0;
//	char* space ="    ";
//	char* spacep = &space;
	x1=x;
	y1=y;

	while(*str!='\0')
	{
	  if(size ==1 )
		{showzifu(x1,y1,*str,dcolor,bgcolor);
			x1+=12;
			}
	  else
	  	{endValue=showzifularge(x1,y1,*str,dcolor,bgcolor);
	    	x1+=endValue+4;
//	     endValue=showzifularge(endValue,y1,*space,BLUEBG,BLUEBG);
//			x1+=endValue;	
			 }
	
		str++;
//		asm("nop");
	}
}



/*
void showzifuA(unsigned int x,unsigned int y,unsigned char value,unsigned int dcolor,unsigned int bgcolor)
{
	unsigned char i,j,a;
	const unsigned char *temp=zikuA;
    Address_set(x,y,x+23,y+39); //????
	temp+=(value-64)*120;
	for(j=0;j<120;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<(7-i)))!=0)
			{
				LCD_WR_DATA(dcolor);
			}
			else
			{
				LCD_WR_DATA(bgcolor);
			}
		}
		temp++;
	 }
}*/
/*
void showzifustrA(unsigned int x,unsigned int y,unsigned char *str,unsigned int dcolor,unsigned int bgcolor)	  
{  
	unsigned int x1,y1,i;
	x1=x;
	y1=y;
	
	while(*str!='\0')
	{
//		showzifuA(x1,y1,*str,dcolor,bgcolor);
		x1+=23;
		str++;
		asm("nop");
	}
}
*/
void inttostr(int dd,char *str)
{
	if((dd/10000)==0)str[0]=32;else str[0]=dd/10000+48;
	if(((dd/10000)==0)&&((dd/1000)==0))str[1]=32; else str[1]=(dd/1000)-((dd/10000)*10)+48;
	if(((dd/10000)==0)&&((dd/1000)==0)&&((dd/100)==0))str[2]=32; else str[2]=(dd/100)-((dd/1000)*10)+48;
	str[3]=(dd/10)-((dd/100)*10)+48;
	str[4]=dd-((dd/10)*10)+48;
	str[5]=0;
}

void inttostrA(int dd,char *str)
{
	if(((dd/10000)==0)&&((dd/1000)==0))str[0]=32; else str[0]=(dd/1000)-((dd/10000)*10)+48;
	if(((dd/10000)==0)&&((dd/1000)==0)&&((dd/100)==0))str[1]=32; else str[1]=(dd/100)-((dd/1000)*10)+48;
	str[2]=(dd/10)-((dd/100)*10)+48;
	str[3]=0x2E;
	str[4]=dd-((dd/10)*10)+48;
	str[5]=0;
}

void inttostrB(int dd,char *str)
{
	if(((dd/10000)==0)&&((dd/1000)==0))str[0]=32; else str[0]=(dd/1000)-((dd/10000)*10)+48;
	str[1]=(dd/100)-((dd/1000)*10)+48;
	str[2]=0x2E;
	str[3]=(dd/10)-((dd/100)*10)+48;
	str[4]=dd-((dd/10)*10)+48;
	str[5]=0;
}

void inttostrC(int dd,char *str)
{
	if(((dd/10000)==0)&&((dd/1000)==0)&&((dd/100)==0))str[0]=32; else str[0]=(dd/100)-((dd/1000)*10)+48;
	str[1]=(dd/10)-((dd/100)*10)+48;
	str[2]=dd-((dd/10)*10)+48;
	str[3]=0x24;
	str[4]=0x43;
	str[5]=0;
}


