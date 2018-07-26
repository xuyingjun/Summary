/*--------------------------------Initialize & Read AI adc results---------------------------*/
#include "adc.h" 

/*----------------------------------local macro definition----------------------------------*/
#define SPI_MISO_AI_PORT	GPIOA
#define SPI_MISO_AI_PIN		GPIO_Pin_6

#define SPI_MOSI_AI_PORT	GPIOA
#define SPI_MOSI_AI_PIN		GPIO_Pin_7

#define SPI_SCLK_AI_PORT	GPIOA
#define SPI_SCLK_AI_PIN		GPIO_Pin_5

#define SPI_CS_AI_0_PORT	GPIOA
#define SPI_CS_AI_0_PIN		GPIO_Pin_4

#define SPI_CS_AI_1_PORT	GPIOA
#define SPI_CS_AI_1_PIN		GPIO_Pin_3

#define SPI_CS_AI_2_PORT	GPIOA
#define SPI_CS_AI_2_PIN		GPIO_Pin_2

#define SPI_CS_AI_3_PORT	GPIOA
#define SPI_CS_AI_3_PIN		GPIO_Pin_1

#define SPI_AI_CS0_CLR()	GPIO_ResetBits(SPI_CS_AI_0_PORT,SPI_CS_AI_0_PIN)
#define SPI_AI_CS0_SET()	GPIO_SetBits(SPI_CS_AI_0_PORT,SPI_CS_AI_0_PIN)

#define SPI_AI_CS1_CLR()	GPIO_ResetBits(SPI_CS_AI_1_PORT,SPI_CS_AI_1_PIN)
#define SPI_AI_CS1_SET()	GPIO_SetBits(SPI_CS_AI_1_PORT,SPI_CS_AI_1_PIN)

#define SPI_AI_CS2_CLR()	GPIO_ResetBits(SPI_CS_AI_2_PORT,SPI_CS_AI_2_PIN)
#define SPI_AI_CS2_SET()	GPIO_SetBits(SPI_CS_AI_2_PORT,SPI_CS_AI_2_PIN)

#define SPI_AI_CS3_CLR()	GPIO_ResetBits(SPI_CS_AI_3_PORT,SPI_CS_AI_3_PIN)
#define SPI_AI_CS3_SET()	GPIO_SetBits(SPI_CS_AI_3_PORT,SPI_CS_AI_3_PIN)

#define SPI_AI_MOSI_CLR()	GPIO_ResetBits(SPI_MOSI_AI_PORT,SPI_MOSI_AI_PIN)
#define SPI_AI_MOSI_SET()	GPIO_SetBits(SPI_MOSI_AI_PORT,SPI_MOSI_AI_PIN)

#define SPI_AI_SCLK_CLR()	GPIO_ResetBits(SPI_SCLK_AI_PORT,SPI_SCLK_AI_PIN)
#define SPI_AI_SCLK_SET()	GPIO_SetBits(SPI_SCLK_AI_PORT,SPI_SCLK_AI_PIN)

#define SPI_AI_MISO_PIN()	((GPIO_ReadInputDataBit(SPI_MISO_AI_PORT,SPI_MISO_AI_PIN)==Bit_SET)?1:0)

#define NUM_ADC 12

typedef struct _adc_bytes{
  uint8_t flag;
  uint8_t integer;
  uint8_t fraction;
}ADC_BYTES;

ADC_BYTES ADC_bytes[12];

void delay_adc(int count);
uint8_t analog_in_read_write_byte(uint8_t c);

/*----------------------------------interface function definition----------------------------------*/
void ADC_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = (SPI_MOSI_AI_PIN) |
		(SPI_SCLK_AI_PIN) |
			(SPI_CS_AI_0_PIN) |
				(SPI_CS_AI_1_PIN) |
					(SPI_CS_AI_2_PIN) |
						(SPI_CS_AI_3_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = (SPI_MISO_AI_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	SPI_AI_SCLK_CLR();
  SPI_AI_CS0_SET();
  SPI_AI_CS1_SET();
  SPI_AI_CS2_SET();
  SPI_AI_CS3_SET();
  delay_adc(2000);	 
}

//channel是通道号，adc_value是存储模拟输入值
void APortIn(char channel,float adc_value[12])
{
	int i;
	uint8_t *p;

	switch(channel/3)
		{
			case 0: SPI_AI_CS3_CLR();p = &(ADC_bytes[0].flag);break;
			case 1: SPI_AI_CS2_CLR();p = &(ADC_bytes[3].flag);break;
			case 2: SPI_AI_CS1_CLR();p = &(ADC_bytes[6].flag);break;
			case 3: SPI_AI_CS0_CLR();p = &(ADC_bytes[9].flag);break;
			default: ;
		}
  delay_adc(2000);    
	for (i = 0; i <9; ++i) 
	{
		p[i] = analog_in_read_write_byte(0xff);    // read 8-bit data
    delay_adc(500);
  }
  SPI_AI_CS0_SET();
  SPI_AI_CS1_SET();
  SPI_AI_CS2_SET();
  SPI_AI_CS3_SET();
	
	if(p[0]==0x01 && p[3]==0x02 && p[6]==0x03)
		{
			if(channel%3 == 0)
				adc_value[channel] = (float)(p[1] + p[2]/100.0);
			else if(channel%3 == 1)
				adc_value[channel] = (float)(p[4] + p[5]/100.0);
			else 
				adc_value[channel] = (float)(p[7] + p[8]/100.0);
		}
	delay_adc(2000);	
}

//adc_value是存储模拟输入值
void APortCIn(float *adc_value)
{
	int i;

	for(i = 0; i < 12; i++)
	{
		APortIn(i,adc_value);
	  delay_adc(2000);
	}
}

/*----------------------------------------local function definition---------------------------------*/
uint8_t analog_in_read_write_byte(uint8_t c)
{
	int i;
  uint8_t res = 0;
  
	SPI_AI_SCLK_CLR();
  for (i = 0; i < 8; ++i)
	{
		res <<= 1;
    if(c&0x80)
			SPI_AI_MOSI_SET();
    else
      SPI_AI_MOSI_CLR();
    delay_adc(500);
    SPI_AI_SCLK_SET();
    delay_adc(500);
    SPI_AI_SCLK_CLR();
    delay_adc(500);
    if(SPI_AI_MISO_PIN())
			res |= 1;			 
    c <<= 1;
   }
	return res;
}

 void delay_adc(int count)
{
	if(count<0)
		count = 0;	
  while(count--);	
}

/*------------------------------------End of source file------------------------------------------*/
