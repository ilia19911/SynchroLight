#include "include.h"

void pwm_input_capture_Init (pwm_input_t *pwmInit)
{
	TIM_TimeBaseInitTypeDef sTIMBase;
	TIM_ICInitTypeDef sIC;
	uint16_t trigger, ccChanel;
	
	// Первоначальная установка таймера. Под вопросом остается разрешение. Но думается мне, что разрешения в 1 мкс будет прекрасно
	sTIMBase.TIM_ClockDivision = TIM_CKD_DIV1;
	sTIMBase.TIM_CounterMode = TIM_CounterMode_Up;
	sTIMBase.TIM_Period = 0xffff;
	sTIMBase.TIM_Prescaler = (F_CPU / 1000000) - 1;
	sTIMBase.TIM_RepetitionCounter = DISABLE;
	TIM_TimeBaseInit (pwmInit->TIMx, &sTIMBase);

	// Настраиваем таймер на вход
	sIC.TIM_Channel = pwmInit->TIM_Channel;
	sIC.TIM_ICFilter = 0;
	if (pwmInit->polarity == PWM_INPUT_POLARITY_RISING)	sIC.TIM_ICPolarity = TIM_ICPolarity_Rising;
	else sIC.TIM_ICPolarity = TIM_ICPolarity_Falling;
	sIC.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	sIC.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_PWMIConfig (pwmInit->TIMx, &sIC);
	
	// Настройки подчиненного таймера
	switch (pwmInit->TIM_Channel)
	{
		case TIM_Channel_1: trigger = TIM_TS_TI1FP1; ccChanel = TIM_IT_CC1; break;
		case TIM_Channel_2: trigger = TIM_TS_TI2FP2; ccChanel = TIM_IT_CC2; break;
		case TIM_Channel_3: trigger = TIM_TS_TI1FP1; ccChanel = TIM_IT_CC3; break;
		case TIM_Channel_4: trigger = TIM_TS_TI2FP2; ccChanel = TIM_IT_CC4; break;
	}
	
	TIM_SelectInputTrigger (pwmInit->TIMx, trigger);
	TIM_SelectSlaveMode (pwmInit->TIMx, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode (pwmInit->TIMx, TIM_MasterSlaveMode_Enable);
	TIM_ARRPreloadConfig (pwmInit->TIMx, ENABLE);
	TIM_ITConfig (pwmInit->TIMx, ccChanel, ENABLE);
	TIM_Cmd (pwmInit->TIMx, ENABLE);
}

void PWM_Input_InterruptCallback (pwm_input_t *pwmInput)
{
	uint16_t ccChanel;
	
	switch (pwmInput->TIM_Channel)
	{
		case TIM_Channel_1:
			pwmInput->duty = TIM2->CCR2;
			pwmInput->period = TIM2->CCR1;
			ccChanel = TIM_IT_CC1;
			break;

		case TIM_Channel_2:
			pwmInput->duty = TIM2->CCR1;
			pwmInput->period = TIM2->CCR2;
			ccChanel = TIM_IT_CC2;
			break;

		case TIM_Channel_3:
			pwmInput->duty = TIM2->CCR4;
			pwmInput->period = TIM2->CCR3;
			ccChanel = TIM_IT_CC3;
			break;

		case TIM_Channel_4:
			pwmInput->duty = TIM2->CCR3;
			pwmInput->period = TIM2->CCR4;
			ccChanel = TIM_IT_CC4;
			break;
	}
	TIM_ClearITPendingBit (pwmInput->TIMx, ccChanel);
}
