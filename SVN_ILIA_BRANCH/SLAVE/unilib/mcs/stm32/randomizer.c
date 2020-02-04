#include "include.h"

void randomizer_init (void)
{
	uint8_t n;
	uint16_t res = 0;
	
	// Инитим АЦП в режим опроса внутреннего температурного датчика
	rcc_adc (ADC1, ENABLE);		
	
	ADC_InitTypeDef sADC;
	
	sADC.ADC_Mode = ADC_Mode_Independent;
	sADC.ADC_ScanConvMode = DISABLE;
	sADC.ADC_ContinuousConvMode = DISABLE;
	sADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	sADC.ADC_DataAlign = ADC_DataAlign_Right;
	sADC.ADC_NbrOfChannel = 1;
	ADC_Init (ADC1, &sADC);
	ADC_Cmd (ADC1, ENABLE);
	
	ADC_ResetCalibration (ADC1);
	while (ADC_GetResetCalibrationStatus (ADC1));
	ADC_StartCalibration (ADC1);
	while (ADC_GetCalibrationStatus (ADC1));
	ADC_TempSensorVrefintCmd (ENABLE);
	
	ADC_RegularChannelConfig (ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd (ADC1, ENABLE);
	
	for (n = 0; n < RANDOMIZER_SAMPLES_NUM; n++)
	{
		while (ADC_GetFlagStatus (ADC1, ADC_FLAG_EOC) == RESET);
		res ^= ADC1->DR;
		res <<= 1;
		
		ADC_SoftwareStartConvCmd (ADC1, ENABLE);
	}
		
	ADC_DeInit (ADC1);
	
	// Инитим генератор псевдослучайный генератор полученным сидом
	srand (res);
}
