#include "stm32f10x.h"
#include "AD.h"
#include "Globle.h"
#include "stm32f10x_adc.h"
#define ADC1_DR_Address ((u32)0x40012400+0x4c)
u32 sum=0;
void ADC1_GPIO_Config(void)
{ 
	GPIO_InitTypeDef 
	GPIO_InitStructure; 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
} 

void ADC1_Mode_Config(void) 
{ 
		DMA_InitTypeDef DMA_InitStructure; 
		ADC_InitTypeDef ADC_InitStructure; 
		DMA_DeInit(DMA1_Channel1); 
		DMA_InitStructure.DMA_PeripheralBaseAddr =ADC1_DR_Address; 
		DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&ADC_ConvertedValue; 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
		DMA_InitStructure.DMA_BufferSize = 60; 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
		DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
		DMA_Cmd(DMA1_Channel1, ENABLE); 

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
		ADC_InitStructure.ADC_NbrOfChannel = 6; 
		ADC_Init(ADC1, &ADC_InitStructure); 

		RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_239Cycles5); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_239Cycles5);

		ADC_DMACmd(ADC1, ENABLE); 

		ADC_Cmd(ADC1, ENABLE); 

		ADC_ResetCalibration(ADC1); 

		while(ADC_GetResetCalibrationStatus(ADC1)); 

		ADC_StartCalibration(ADC1); 

		while(ADC_GetCalibrationStatus(ADC1)); 

		ADC_SoftwareStartConvCmd(ADC1, ENABLE); 

}
float ReadAdOfChannel(u8 ch,u8 mode)
{
	int i;
	
	sum=0;
	for(i=0;i<10;i++)
	{
		sum+=ADC_ConvertedValue[i][ch-1];
	}
	if(mode==MODE_CURRENT)
	{
		return (2.50*sum/10/4096*(13.0/3)*1000/(196.97));
	}
	else
	{
		return (2.50*sum/10/4096*(13.0/3));
	}
}
