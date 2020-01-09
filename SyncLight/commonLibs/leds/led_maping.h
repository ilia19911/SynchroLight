#include "include.h"

#include "ws2812b.h"


#ifndef _LED_MAPING_H
#define _LED_MAPING_H

#define X_MATRIX_SIZE  3// ������ ������� �� �����������

#define Y_MATRIX_SIZE  35// 	������ ������� �� ���������
#define TRUE 	0x01
#define FALSE 0x00


typedef struct 
{
	uint8_t matrix[X_MATRIX_SIZE][Y_MATRIX_SIZE+1];
	leds *my_leds;
}led_map;

void  		led_maping_init_leds(leds *this_leds);// назначает массив светодиодов для матрицы
void	 	led_maping_set_map(void);// установка матрицы. пока тестово заполняем матрицу из статического массива
uint8_t 	led_map_set_pixel(uint8_t x, uint8_t y, leds this_leds);// запись состояния светодиода
leds 		get_led_state(uint8_t x, uint8_t y); 
void 		led_maping_temp_matrix_set(void);

#endif

