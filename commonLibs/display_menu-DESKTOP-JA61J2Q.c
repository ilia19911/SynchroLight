#include "include.h"
#include "stdio.h"
uint8_t page_number;
uint16_t this_channel;

void menu_init(void)
{
		page_number 	= 0;
		this_channel 	= channel;
}
static void main_page_hanler()
{		
		uint8_t presset_button = buttonTask();
		char string[5];
		sprintf(string, "  %2d", my_player.frame );
		display_set_string( string);
		if(presset_button == button_1) 
		{
				player_play_mem(0, 0,	0);
		}
		if(presset_button == button_3) 
		{
				player_stop_mem();
		}
		if(presset_button == button_4) 
		{
				page_number	=	rf_bind_page;
		}
}

static void rf_page_handler()
{
		char string[5];
		uint8_t presset_button = buttonTask();

		sprintf(string, "01%2d", this_channel );
		display_set_string( string);

		if(presset_button == button_1) 
		{
				rf_binding(this_channel);
		}
<<<<<<< .mine
		if(presset_button == button_2) 
		{
				this_channel--;
				if(this_channel<0)this_channel=0;
		}
		if(presset_button == button_3) 
		{
				this_channel++;
				if(this_channel>9)this_channel=9;
		}
||||||| .r96
=======
		if(presset_button == button_2) 
		{
				if(this_channel != 0)
				{
						this_channel--;
				}
		}
		if(presset_button == button_3) 
		{
				this_channel++;
				if(this_channel>9)this_channel=9;
		}
>>>>>>> .r101
		if(presset_button == button_4) 
		{
				page_number	=	main_page;
		}
		
}
void menu_handler(void)
{
	dislay_Handler();

	switch(page_number)
	{
		case main_page:
			main_page_hanler();
			break;
		case rf_bind_page:
			rf_page_handler();
			break;
	}
}


