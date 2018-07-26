#include "UI.h"
#include <string.h>
#include "LCD.h"


extern u8 Startflag;
void PAGE0(void)
{
	LCD_Main_Clear(BLUEBG);

	LCD_Fill(238,0,242,479,0X01CF);//
	LCD_Fill(242,138,479,142,0X01CF);//

	LCD_Fill(0,0,479,35,0X01CF);//BSTAR
	LCD_Fill(0,245,479,281,0X01CF);//function


//	LCD_Fill(242,45,242,281,0X01CF);//
	LCD_Fill(242,35,243,245,0xB723);//

	LCD_Fill(242,138,479,142,0X01CF);//
	LCD_Fill(242,142,479,143,0xB723);//

	LCD_Fill(242,245,380,281,0X01CF);//

	LCD_Fill(0,244,479,245,0xB723);//
	LCD_Fill(0,35,479,36,0xB723);//
	showzifustr(10,8,"AIR T&D",0xB723,0X01CF,1);

	showzifustr(20,120,"PV:",0x0000,BLUEBG,1);
	showzifustr(20,200,"SV:",0x0000,BLUEBG,1);

	showzifustr(20,50,"Temperature:",0x0000,BLUEBG,1);
    showzifustr(262,50,"RH:",0x0000,BLUEBG,1);
	showzifustr(262,158,"CO#:",0x0000,BLUEBG,1);

}

unsigned char PAGE1(int PvRH,int PvTe,int SvTe,int PvCO2)
{
	char ss[6];	
	unsigned char i=0;			    

 
 //	showzifustr(127,247,"CONFIG",0xFFFF,0X01CF,1);  //dehumidifier
	
	inttostrA(PvTe,ss);
	showzifustr(70,110,ss,0,BLUEBG,2);
	showzifustr(190,110,"$C",0,BLUEBG,1);

    inttostrA(SvTe,ss);
	showzifustr(70,200,ss,0,BLUEBG,1);
	showzifustr(190,200,"$C",0,BLUEBG,1);

	inttostr(PvCO2,ss);
	showzifustr(302,188,ss,0,BLUEBG,1);
	showzifustr(370,188,"ppm",0,BLUEBG,1);

	inttostrA(PvRH,ss);
	showzifustr(302,90,ss,0,BLUEBG,1);
    showzifustr(370,90,"%",0,BLUEBG,1);
    
	i=1;
	return i;
}
 
//unsigned char PAGE2(int SvTe,int SvCO2)
//{
//	char ss[6];
//	unsigned char i=0;	
//	LCD_Main_Clear(BLUEBG);
//	
//	showzifustr(120,247,"  BACK  ",0xffff,0X01CF,1);
//	showzifustr(242,247,"  SAVE  ",0xffff,0X01CF,1);
//	showzifustr(10,247,"ON",0xFFFF,0X01CF,1);
//	showzifustr(410,247,"OFF",0xFFFF,0X01CF,1);
//	showzifustr(335,8,"Emergency",0X01CF,0xB723,1);
//
//	LCD_Fill(238,0,242,479,0X01CF);//
//    LCD_Fill(242,45,243,235,0xB723);//
//   	LCD_Fill(0,234,479,235,0xB723);//
//	LCD_Fill(0,45,479,46,0xB723);//
//
//	showzifustr(20,60,"Temperature $C",0x0000,BLUEBG,1);
//    showzifustr(272,60,"  CO# ppm",0x0000,BLUEBG,1);
//
//	showzifustr(10,140,"<",0xB723,BLUEBG,1);
//	showzifustr(200,140,">",0x7BCF,BLUEBG,1);
//	showzifustr(260,140,"<",0xB723,BLUEBG,1);
//	showzifustr(450,140,">",0x7BCF,BLUEBG,1);
//
//	inttostrA(SvTe,ss);
//	showzifustr(70,120,ss,0,BLUEBG,2);
//	inttostr(SvCO2,ss);
//	showzifustr(312,120,ss,0,BLUEBG,2);
//
//	i=1;
//	return i;
//
//}
//
//unsigned char PAGE3(void)
//{
////	char ss[6];
//	unsigned char i=0;	
//	LCD_Main_Clear(BLUEBG);
//	
//	LCD_Fill(0,0,479,45,0X01CF);//BSTAR
//	LCD_Fill(0,235,479,281,0X01CF);//function
//	LCD_Fill(238,0,242,479,0X01CF);//
//
//	LCD_Fill(0,234,479,235,0xB723);//
//	LCD_Fill(0,45,479,46,0xB723);//
//   	LCD_Fill(242,45,243,235,0xB723);//
//
//
//
//	showzifustr(10,8,"AIR T&D",0xB723,0X01CF,1);
//	showzifustr(10,120,"START",0x0000,BLUEBG,2);
//	showzifustr(280,120,"STOP",RED,BLUEBG,2);
//	if(Startflag)
//   	showzifustr(395,247,"Return",0xffff,0X01CF,1);
//    else
//	showzifustr(395,247,"Return",0x5ACB,0X01CF,1);
//
//	i=3;
//	return i;
//
//}
