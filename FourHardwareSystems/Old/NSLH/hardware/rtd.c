#include "rtd.h"
#define SPI_MISO_RTD_PORT	GPIOG
#define SPI_MISO_RTD_PIN		GPIO_Pin_14

#define SPI_MOSI_RTD_PORT	GPIOG
#define SPI_MOSI_RTD_PIN		GPIO_Pin_15

#define SPI_SCLK_RTD_PORT	GPIOG
#define SPI_SCLK_RTD_PIN		GPIO_Pin_13

#define SPI_CS_RTD_0_PORT	GPIOB
#define SPI_CS_RTD_0_PIN		GPIO_Pin_7

#define SPI_CS_RTD_1_PORT	GPIOB
#define SPI_CS_RTD_1_PIN		GPIO_Pin_8

#define SPI_RTD_CS0_CLR()	GPIO_ResetBits(SPI_CS_RTD_0_PORT,SPI_CS_RTD_0_PIN)
#define SPI_RTD_CS0_SET()	GPIO_SetBits(SPI_CS_RTD_0_PORT,SPI_CS_RTD_0_PIN)

#define SPI_RTD_CS1_CLR()	GPIO_ResetBits(SPI_CS_RTD_1_PORT,SPI_CS_RTD_1_PIN)
#define SPI_RTD_CS1_SET()	GPIO_SetBits(SPI_CS_RTD_1_PORT,SPI_CS_RTD_1_PIN)

#define SPI_RTD_MOSI_CLR()	GPIO_ResetBits(SPI_MOSI_RTD_PORT,SPI_MOSI_RTD_PIN)
#define SPI_RTD_MOSI_SET()	GPIO_SetBits(SPI_MOSI_RTD_PORT,SPI_MOSI_RTD_PIN)

#define SPI_RTD_SCLK_CLR()	GPIO_ResetBits(SPI_SCLK_RTD_PORT,SPI_SCLK_RTD_PIN)
#define SPI_RTD_SCLK_SET()	GPIO_SetBits(SPI_SCLK_RTD_PORT,SPI_SCLK_RTD_PIN)

#define SPI_RTD_MISO_PIN()	((GPIO_ReadInputDataBit(SPI_MISO_RTD_PORT,SPI_MISO_RTD_PIN)==Bit_SET)?1:0)

#pragma pack(1)
typedef struct _rtd_bytes
{
    uint8_t b55;
    uint8_t baa;
    int32_t temp[4];     //每个测温模块连接两个测温传感器
    uint8_t  nuse1;
    uint8_t  nuse2;
}RTD_BYTES;
#pragma pack()

RTD_BYTES rtd_bytes;	  // 存放两个测温模块的数据，共4个温度传感器。
const uint8_t cmd[12] = {0x01, 0x02, 0x03};

uint8_t rtd_read_write_byte(uint8_t c);
void delay_rtd(void);
/*----------------------------------interface function definition----------------------------------*/
void RTD_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = (SPI_MOSI_RTD_PIN) | (SPI_SCLK_RTD_PIN) ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = (SPI_MISO_RTD_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin =  (SPI_CS_RTD_0_PIN) | (SPI_CS_RTD_1_PIN) ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	SPI_RTD_SCLK_CLR();
  delay_rtd();
  SPI_RTD_CS0_SET();
  SPI_RTD_CS1_SET();
}

//channel是通道号，temperature是存储温度数据
void RTD_onechannel(int channel,float* temperature)
{
	uint8_t *dp;   
  int i;
  
	delay_rtd();
  dp = (void *)& rtd_bytes.b55;
  SPI_RTD_CS0_CLR();
  delay_rtd();
  for (i = 0; i < sizeof(RTD_BYTES); ++i) 
	{
		dp[i] = rtd_read_write_byte(cmd[i]);
    delay_rtd();
  } 
  SPI_RTD_CS0_SET();
	if(channel<4 && channel>=0)
		temperature[channel] =  rtd_bytes.temp[channel]/1000.0;
}

//全通道一次性存储温度数据
void RTD_allchannels(float* temperature)
{
	uint8_t *dp;   
  int i;
  
	delay_rtd();
  dp = (void *)& rtd_bytes.b55;
  SPI_RTD_CS0_CLR();
  delay_rtd();
  for (i = 0; i < sizeof(RTD_BYTES); ++i) 
	{
		dp[i] = rtd_read_write_byte(cmd[i]);
    delay_rtd();
  } 
  SPI_RTD_CS0_SET();
	for(i=0;i<4;i++)
	{
		temperature[i] =  rtd_bytes.temp[i]/1000.0;
	}
}

/*----------------------------------------local function definition---------------------------------*/
uint8_t rtd_read_write_byte(uint8_t c)
{
	int i;
  uint8_t res = 0;
  
	for (i = 0; i < 8; ++i) 
	{
		res <<= 1;
    SPI_RTD_SCLK_SET();
    if(c&0x80)
			{
				SPI_RTD_MOSI_SET();
      }
    else
			{
				SPI_RTD_MOSI_CLR();
      }
        
   delay_rtd();
   SPI_RTD_SCLK_CLR();
   delay_rtd();
   if(SPI_RTD_MISO_PIN())
		 {
			 res |= 1;
     }
   c <<= 1;
  }
 return res;
}

void delay_rtd(void)
{
	volatile int ___j = 500;	
  while(___j--);	
}
