#include "touch.h"

					
 struct tp_pix_  tp_pixad,tp_pixlcd;
 	 			  
void TP_Init(void)
{			    		   
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//CS  SCLK MOSI 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//MISO INT 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

 									 
}

//num:要写入的数据
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 	 
		TCLK=1;		//上升沿有效	        
	}		 			    
} 

u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//先拉低时钟 	 
	TDIN=0; 	//拉低数据线
	TCS=0; 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		//给1个时钟，清除BUSY	    	    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	//下降沿有效  	    	   
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//只有高12位有效.
	TCS=1;		//释放片选	 
	return(Num);   
}


u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 


u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}

//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}

unsigned char Convert_Pos(void)
{
	unsigned char l=0;
	if(TP_Read_XY2(&tp_pixad.x,&tp_pixad.y)==1)
	{
		l=1;
		tp_pixlcd.x=tp_pixad.x>chx?((unsigned long)tp_pixad.x-chx)*1000/vx:(chx-(unsigned long)tp_pixad.x)*1000/vx;
		tp_pixlcd.y=tp_pixad.y>chy?((unsigned long)tp_pixad.y-chy)*1000/vy:(chy-(unsigned long)tp_pixad.y)*1000/vy;
	}
	return l;
}

char function(void)
{
	 Convert_Pos();

	 if((((tp_pixlcd.x)>0)&&((tp_pixlcd.x)<60))&&(((tp_pixlcd.y)>230)&&((tp_pixlcd.y)<281)))        return 1;
	 else if((((tp_pixlcd.x)>100)&&((tp_pixlcd.x)<180))&&(((tp_pixlcd.y)>230)&&((tp_pixlcd.y)<281)))return 2;
	 else if((((tp_pixlcd.x)>230)&&((tp_pixlcd.x)<290))&&(((tp_pixlcd.y)>230)&&((tp_pixlcd.y)<281)))return 3;
	 else if((((tp_pixlcd.x)>400)&&((tp_pixlcd.x)<480))&&(((tp_pixlcd.y)>230)&&((tp_pixlcd.y)<281)))return 4;

	 else if((((tp_pixlcd.x)>335)&&((tp_pixlcd.x)<470))&&(((tp_pixlcd.y)>0)&&((tp_pixlcd.y)<40)))   return 5;

	 else if((((tp_pixlcd.x)>10)&&((tp_pixlcd.x)<200))&&(((tp_pixlcd.y)>110)&&((tp_pixlcd.y)<180))) return 8;  //
	 else if((((tp_pixlcd.x)>280)&&((tp_pixlcd.x)<400))&&(((tp_pixlcd.y)>110)&&((tp_pixlcd.y)<180)))return 9;
	 
	 else if((((tp_pixlcd.x)>0)&&((tp_pixlcd.x)<60))&&(((tp_pixlcd.y)>120)&&((tp_pixlcd.y)<180)))   return 10;  //
	 else if((((tp_pixlcd.x)>170)&&((tp_pixlcd.x)<210))&&(((tp_pixlcd.y)>120)&&((tp_pixlcd.y)<180)))return 11;
	 else if((((tp_pixlcd.x)>260)&&((tp_pixlcd.x)<310))&&(((tp_pixlcd.y)>120)&&((tp_pixlcd.y)<180)))return 12;
	 else if((((tp_pixlcd.x)>430)&&((tp_pixlcd.x)<470))&&(((tp_pixlcd.y)>120)&&((tp_pixlcd.y)<180)))return 13;
	 
	 
	 else return 99;
}

void touch_clear(void)
{
	tp_pixlcd.x=0;
	tp_pixlcd.y=0;
}
