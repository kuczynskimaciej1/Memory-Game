#include<stm32f10x_rcc.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_adc.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

void adc_init(void)
{
	// input of ADC (it doesn't seem to be needed, as default GPIO state is floating input)
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 ;		// that's ADC1 (PC4 on STM32)
		GPIO_Init(GPIOC, &GPIO_InitStructure);

	//clock for ADC (max 14MHz --> 72/6=12MHz)
		RCC_ADCCLKConfig (RCC_PCLK2_Div6);
	// enable ADC system clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// define ADC config
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;

		ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 1,ADC_SampleTime_28Cycles5); // define regular conversion config
		ADC_Init ( ADC1, &ADC_InitStructure);	//set config of ADC1

	// enable ADC
		ADC_Cmd (ADC1,ENABLE);	//enable ADC1

	//	ADC calibration (optional, but recommended at power on)
		ADC_ResetCalibration(ADC1);	// Reset previous calibration
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
		while(ADC_GetCalibrationStatus(ADC1));

	// start conversion
		ADC_Cmd (ADC1,ENABLE);	//enable ADC1
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)

	// debug information
		RCC_ClocksTypeDef forTestOnly;
		RCC_GetClocksFreq(&forTestOnly);	//this could be used with debug to check to real speed of ADC clock
}


uint16_t adc_scan(void)
{
	uint16_t j;
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);					// start ONE conversion
     while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);		// wait end of conversion
     j = ADC_GetConversionValue(ADC1);							// get value
     return j;
}
