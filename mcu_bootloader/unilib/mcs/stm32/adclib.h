#ifndef _ADCLIB_H
#define _ADCLIB_H

#if F1_CHECK
#include "stm32f10x_adc.h"
#elif F4_CHECK
#include "stm32f4xx_adc.h"
#else
#error "This mcu is not supported, or select mcu type"
#endif

#define INJECTED_CHANNEL_MAX								3

void ADC_DMAChannelInit (ADC_TypeDef* ADCx, uint8_t convClocks, uint16_t *pointer, uint8_t chNum, ...);
uint32_t* ADC_InjectedAddToRotation (ADC_TypeDef* ADCx, uint8_t channel, uint8_t convClocks);

#endif
