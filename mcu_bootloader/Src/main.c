#define MAIN

#include "include.h"

uint32_t c = 0;

int main (void)
{
	initLowLevel ();       
	systick_timer_init ();
	bootloader_init (&boot_uart);
	
	while (1)
	{
		bootloader_task ();
		c++;
	}
}
