#define MAIN

#include "include.h"

timeout_t test;

int main (void)
{
#ifdef NDEBUG
	__disable_irq();
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08002000);
	__enable_irq();
#endif /* DEBUG */

	
	initLowLevel ();     
	randomizer_init ();	
	systick_timer_init ();
	fileStorageInit ();
	rf_init ();
	menu_init();

	software_timer_start (&test, 5000);
	
	// Если в момент инициализации есть флэшка в слоте -- посмотрим, что на ней есть и если что-то интересное - скопируем себе в память
	if (!pin_test (PIN_FLASH_CARD_DET)) sdTask ();
	
	player_play_mem(0, 0,	0);
	
	// Включаем вачдог
	wdt_enable (3.0f);
	
	while (1)
	{
		payer_handler();
		rf_task ();
		menu_handler();
		
		wdt_reset();
	}
}
