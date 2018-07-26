#include "dac.h"

#define DAC_PORT	GPIOB

#define DAC_SPI_MOSI_PORT	DAC_PORT
#define DAC_SPI_MOSI_PIN	GPIO_Pin_13

#define DAC_SPI_MISO_PORT	DAC_PORT
#define DAC_SPI_MISO_PIN	GPIO_Pin_14

#define DAC_SPI_SCLK_PORT	DAC_PORT
#define DAC_SPI_SCLK_PIN	GPIO_Pin_15

#define DAC_SPI_CS_PORT		DAC_PORT
#define DAC_SPI_CS_PIN		GPIO_Pin_12

#define DAC_SPI_CS_CLR()	GPIO_ResetBits(DAC_SPI_CS_PORT,DAC_SPI_CS_PIN)
#define DAC_SPI_CS_SET()	GPIO_SetBits(DAC_SPI_CS_PORT,DAC_SPI_CS_PIN)

#define DAC_SPI_MOSI_CLR()	GPIO_ResetBits(DAC_SPI_MOSI_PORT,DAC_SPI_MOSI_PIN)
#define DAC_SPI_MOSI_SET()	GPIO_SetBits(DAC_SPI_MOSI_PORT,DAC_SPI_MOSI_PIN)

#define DAC_SPI_SCLK_CLR()	GPIO_ResetBits(DAC_SPI_SCLK_PORT,DAC_SPI_SCLK_PIN)
#define DAC_SPI_SCLK_SET()	GPIO_SetBits(DAC_SPI_SCLK_PORT,DAC_SPI_SCLK_PIN)

#define DAC_SPI_MISO()	((GPIO_ReadInputDataBit(DAC_SPI_MISO_PORT,DAC_SPI_MISO_PIN)==Bit_SET)?1:0)


#define NOP  (0x00)
#define Write_DAC_Data  (0x01)
#define Register_read   (0x02)
#define Write_control   (0x55)
#define Write_reset     (0x56)
#define Write_configuration (0x57)
#define Write_DAC_gain_calibration  (0x58)
#define Write_DAC_zero_calibration  (0x59)
#define Watchdog_timer_reset        (0x95)

#define NUM_DAC	8

struct _dac_reg
{
	uint8_t addr;
	uint16_t reg;
};

#define delay()	do{\
	int ___j = 2;	\
	while(___j--);	\
}while(0)

struct _dac_reg dac_out[NUM_DAC];
void dac_write(struct _dac_reg *conf);
uint8_t dac_read_write_byte(uint8_t c);
/*----------------------------------interface function definition----------------------------------*/
void DAC_init(void)
{
	int i;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = (DAC_SPI_MOSI_PIN) |
								  (DAC_SPI_SCLK_PIN) |
								  (DAC_SPI_CS_PIN) ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DAC_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (DAC_SPI_MISO_PIN);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DAC_PORT, &GPIO_InitStructure);

	DAC_SPI_CS_SET();

	for (i = 0; i < NUM_DAC; ++i)
	{
		dac_out[i].addr = Write_control;
		dac_out[i].reg = 0x1009;
	}
	
	for (i = 0; i < NUM_DAC; ++i)
	{
		dac_write(&dac_out[0]);
	}

	for (i = 0; i < NUM_DAC; ++i)
	{
		dac_out[i].addr = Write_configuration;
		dac_out[i].reg = 0x0380;
	}
	dac_write(&dac_out[0]);

  for (i = 0; i < NUM_DAC; ++i)
	{
		dac_out[i].addr = Write_DAC_Data;
		dac_out[i].reg = 0x00;
	}
	dac_write(&dac_out[0]);
}

//channel 是通道，value是输出值，0-10V
void APortOut(uint8_t channel, float value)
{
	int i; 	
	if(value>10)
		value = 10;
	else if(value<0)
		value = 0;

	for (i = 0; i < NUM_DAC; ++i)
	{
		dac_out[i].addr = Write_DAC_Data;
	} 
	dac_out[channel].reg = (uint16_t)(6553.5 * value + 0.5); 
	dac_write(&dac_out[0]);	 
}

//value是端口输出值
void APortCOut(float* value)
{
	int i;
	for (i = 0; i < NUM_DAC; ++i) 
	{
		dac_out[i].addr = Write_DAC_Data;
		dac_out[i].reg = (uint16_t)(6553.5 * value[i] + 0.5);
	}
	dac_write(&dac_out[0]);
}

/*----------------------------------local function definition----------------------------------*/
 uint8_t dac_read_write_byte(uint8_t c)
{
	int i;
	uint8_t res = 0;

	DAC_SPI_SCLK_CLR();
	for (i = 0; i < 8; ++i)
	{
		res <<= 1;
		if(c&0x80)
			DAC_SPI_MOSI_SET();
		else
			DAC_SPI_MOSI_CLR();
		DAC_SPI_SCLK_SET();
		if(DAC_SPI_MISO())
			res |= 1;
		DAC_SPI_SCLK_CLR();
		c <<= 1;
	}
	return res;
}

void dac_write(struct _dac_reg *conf)
{
	int i;

	DAC_SPI_CS_CLR();

	for (i = 0; i < NUM_DAC; ++i)
	{
		dac_read_write_byte(conf[i].addr);
		dac_read_write_byte(conf[i].reg>>8);
		dac_read_write_byte(conf[i].reg&0xFF);
	}

	DAC_SPI_CS_SET();
}
