


#include "io_spi.h"
#include "delay.h"


/**********************************/
/*      Interface Define          */
/**********************************/

/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */




uint8_t   IO_SPI_read_write_data(uint8_t data);



/**********************************/
/*      Time Delay                */
/**********************************/


void IO_SpiInit(void)
{
	
		GPIO_InitTypeDef  GPIO_InitStructure;


  /* set CSn to default high level */
  IO_SPI_DRIVE_CSN_1_HIGH();
  IO_SPI_DRIVE_CSN_2_HIGH();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  IO_SPI_DRIVE_SCLK_LOW();
  
} 


void delayNum(void)
{
	int i,j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			
		}
	}
}


uint8_t IO_SPI_read_write_data(uint8_t data)
{
  int i,j;
  uint8_t data_in = 0;
	j = 0x80;
  for(i=0;i<8;i++)
     {
          
        IO_SPI_DRIVE_SCLK_HIGH();
        if(data&j){
            IO_SPI_DRIVE_SI_HIGH();  
        }else {
            IO_SPI_DRIVE_SI_LOW();  
        }
				delayNum();
        IO_SPI_DRIVE_SCLK_LOW();  
				delayNum();
        data_in <<= 1;
        if(IO_SPI_SO_IS_HIGH()){
            data_in |= 0x01;
        }
        j = j>>1;
     }
  return data_in;
}

