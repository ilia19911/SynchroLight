#include "include.h"
#ifndef _EFFECT_PROCESSOR_H
#define _EFFECT_PROCESSOR_H

//инструкции воспроизведения сцен
#define LINEAR_COLOR_CHANGE 1

//инструкции форматов сцен
#define ABSOLUTE_COLOR 			1
#define INDIVIDUAL_X_COLOR 	2

//
void linear_color_change_handler(frame instruction, uint16_t x, uint16_t y, leds *this_led, uint8_t crosfade);
void handler_definition(frame instruction, uint16_t x, uint16_t y, leds *this_led, uint8_t crosfade);


//структура для заполнения данных линейного перехода
#pragma pack(push, 1)
typedef struct 
{
	//leds led_state;
	uint16_t	route;
	uint8_t fade;
	uint8_t scen[];
	
}linear_process;
#pragma pack(pop, 1)


////////////////////////////////////////////////////структуры форматов
//структура формата абсолютного цвета
#pragma pack(push, 1)
typedef struct 
{
	//leds led_state;
	uint16_t	format;
	leds 			led_state;
	
}absolute_color_scen;
#pragma pack(pop, 1)

#pragma pack(push, 1)
typedef struct 
{
	//leds led_state;
	uint16_t	format;
	uint8_t 	line_number;
	leds 			led_state[Y_MATRIX_SIZE];
}individual_x_color_scen;
#pragma pack(pop, 1)

#endif

