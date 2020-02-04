#ifndef _SI7007_H_
#define _SI7007_H_

#define HUMIDITY_K										((float)125)
#define HUMIDITY_B										((float)-6)

#define TEMPERATURE_K									((float)175.72)
#define TEMPERATURE_B									((float)-46.85)
	
typedef struct 
{
	pwm_input_t h_pwm;
	pwm_input_t t_pwm;
	float humidity;
	float temperature;
} si7007_t;

void si7007_Init (si7007_t *init);
void si7007Task (si7007_t *sens);

#endif /* _SI7007_H_ */
