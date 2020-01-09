#include "include.h"

void si7007_Init (si7007_t *init)
{
	if (init->h_pwm.TIMx) pwm_input_capture_Init (&init->h_pwm);
	if (init->t_pwm.TIMx) pwm_input_capture_Init (&init->t_pwm);
}

void si7007Task (si7007_t *sens)
{
	float temp;
	
	if (sens->h_pwm.TIMx)
	{
		temp = (float)sens->h_pwm.duty / (float)sens->h_pwm.period;
		sens->humidity = HUMIDITY_B + HUMIDITY_K * temp;
	}
	
	if (sens->t_pwm.TIMx)
	{
		temp = (float)sens->t_pwm.duty / (float)sens->t_pwm.period;
		sens->temperature = TEMPERATURE_B + TEMPERATURE_K * temp;
	}
}
