#include "stm32f10x.h"
#include "DHT11.h"
#include "sys.h"
#include "stm32f10_timer.h"
//unsigned char  humidity, temperature, check_sum ; 
u8 tdata[4]={0x00,0x00,0x00,0x00};
u8 sbuf,check;

                    //ȫ�ֱ���������
//***************************************************************************/
//      //uchar       i;
//      uchar    U8FLAG,k;
//      uchar    U8count,U8temp;
//      uchar    U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
//      uchar    U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
//      uchar    U8comdata;
//      uint   ReceiveHighByte;
//      uint   ReceiveLowByte;
//***************Global defination for DHT11 end****// 
void DHT11_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void DHT11_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
unsigned char StartDHT11(void)
{
   //unsigned char flag;

   DHT11_PortOUT();
   DHT11_Out = 0;
   delay_ms(18);  //������ʼ,��������Ҫ����18ms
  
   DHT11_Out = 1;	//�ͷ�����
   delay_us(25);  //wait 20-40uS �ȴ�DHT11�ĵ͵�ƽ��Ӧ�ź�
   
       
   DHT11_PortIN();//��Ϊ����
   //delay_us(5);   //
   //if(!PINC.3)
   if(!DHT11_In)
   {
	   //while(DHT11_In)//DHT11�����Ӧ�Ļ�����������80us
	   //{
	   	   while(!DHT11_In);//�͵�ƽ����Ӧ�źţ�80us
		   while(DHT11_In);//��������80us�ĸߵ�ƽ����׼���ź�
		   //���ͺ�DHT11����������80us,���ŻῪʼ������	
		   return 1;
	   //}   
   }
//   if(DHT11_In)
//   {
//      //while((!PINC.3) && flag ++); //�ȴ��͵�ƽ80uS
//         
//      while((!DHT11_In) && flag ++);
//      //while((PINC.3) && flag ++);  //�ȴ��ߵ�ƽ80uS
//         
//	  while((DHT11_In) && flag ++);
//      return(1);
//   }
	return 0;
}
void com(void)
{
	u8 i,tt;
	//Flag = 0;
	tt = 0;
	for(i = 0;i<8;i++)
	{
		sbuf <<= 1;
		delay_us(20);//���յ���Ӧ������50us�ĵ͵�ƽ��ʾ�������ݵĿ�ʼ����������С��ʱһ��
		while(!DHT11_In);//�ȵ��ߵ�ƽ�ĳ��֣��ߵ�ƽ��ʱ���ʾ��������λ��0��1
		delay_us(25);//����Ϊ0���ź�ʱ��Ϊ26-28us��1��Ϊ70us�����ﳬʱ���
		tt = 100;
		while(tt++);
		//tt = 200;
		//while(tt++);
		if(DHT11_In)//�����Ϊ��
		{
			 sbuf |= 0x01;
			 //while(DHT11_In && ++Flag);//�ȴ�����Ϊ�͵�ƽ�ź� ,���ﲻ��һֱ���ȣ���Ϊ���ݴ������˻��������ߵ�
			 delay_us(30);//�������ʱ�㹻�ˣ�40+10+30>70��
		}
		else //������
		{
			sbuf &= 0xfe;
		}
	}
}
u8 ReadDHT11(void)
{
	u8 sum;
	if(StartDHT11())	//�ж��Ƿ��Ѿ���Ӧ
	{
		com();
		tdata[0]=sbuf;
		com();
		tdata[1]=sbuf;
		com();
		tdata[2]=sbuf;		
		com();
		tdata[3]=sbuf;
		com();
		check = sbuf;
		sum = (tdata[0]+tdata[1]+tdata[2]+tdata[3]);
		//sum = sum - 0x80;
	}
	if(check == sum)
	return(1);
	else	
	return 0;
}

