

#ifndef io_spi
#define io_spi

#include "stm32f10x.h"
/* CSn Pin Configuration */
#define IO_SPI_DRIVE_CSN_1_HIGH()             GPIO_SetBits(GPIOC, GPIO_Pin_9)  
#define IO_SPI_DRIVE_CSN_1_LOW()              GPIO_ResetBits(GPIOC, GPIO_Pin_9)  

#define IO_SPI_DRIVE_CSN_2_HIGH()             GPIO_SetBits(GPIOC, GPIO_Pin_10) 
#define IO_SPI_DRIVE_CSN_2_LOW()              GPIO_ResetBits(GPIOC, GPIO_Pin_10) 


#define IO_SPI_DRIVE_SCLK_HIGH()            GPIO_SetBits(GPIOB, GPIO_Pin_13) 
#define IO_SPI_DRIVE_SCLK_LOW()             GPIO_ResetBits(GPIOB, GPIO_Pin_13) 

#define IO_SPI_DRIVE_SI_HIGH()              GPIO_SetBits(GPIOB, GPIO_Pin_15) 
#define IO_SPI_DRIVE_SI_LOW()               GPIO_ResetBits(GPIOB, GPIO_Pin_15) 

#define IO_SPI_SO_IS_HIGH()                 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)



void IO_SpiInit(void);
uint8_t IO_SPI_read_write_data(uint8_t data);



#endif
