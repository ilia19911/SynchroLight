#ifndef _PWM_INPUT_CAPTURE_H_
#define _PWM_INPUT_CAPTURE_H_

typedef enum
{
	PWM_INPUT_POLARITY_RISING,
	PWM_INPUT_POLARITY_FALLING
} pwm_input_polarity_t; 

typedef struct
{
	TIM_TypeDef *TIMx;
	uint16_t TIM_Channel;
	pwm_input_polarity_t polarity;
	uint16_t period;
	uint16_t duty;
} pwm_input_t;

void pwm_input_capture_Init (pwm_input_t *pwmInit);
void PWM_Input_InterruptCallback (pwm_input_t *pwmInput);
	
#endif /* _PWM_INPUT_CAPTURE_H_ */
