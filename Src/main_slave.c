#define MAIN

#include "include.h"
extern uni_eeprom_t settings;
int main (void)
{
	initLowLevel ();  

	randomizer_init ();	
	systick_timer_init ();
	fileStorageInit ();
	rf_init ();
	ws2812_init_univerces();
	
	//delay_ms(100);
	//ws2812b_Init(&my_ws2812_3,70,105); //допилено, чтобы решить проблему с 3м потоком  
	
	led_maping_init_leds(static_led);// назначение массива светодиодов для матрицы
	led_maping_set_map();			 // назначение матрцы светодиодов. Пока тестово вычитываем из статического массива
	
	player_play_mem(0, 0, 0); 

//	rf_set_channel (0);
/* ============== INSERT 1 ============== */	

/* ============== INSERT 1 ============== */	

	while (1)
	{
		
		//transceiverTask ();
		rf_task();
		payer_handler();

/* ============== INSERT 2 ============== */

		
/* ============== INSERT 2 ============== */
	}
}


