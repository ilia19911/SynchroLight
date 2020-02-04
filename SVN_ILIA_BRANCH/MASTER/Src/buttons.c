#include "include.h"

static timeout_t button_tim;
static uint8_t buttons[BUTTONS_NUM] = {0};

void buttonsInit (void)
{
	software_timer_start (&button_tim, BUTTON_TAST_PERIOD_MS);
}

uint8_t buttonTask (void)
{
		if (software_timer (&button_tim))
		{
			if (buttons[0] == 0)
			{
				if (pin_test (PIN_BUTTON_1) == 0) buttons[0] = 1;
				else buttons[0] = 0;
			}
			else
			{
				if (pin_test (PIN_BUTTON_1) == 1) 
				{
					dprintf ("Send scene start cmd");
					//player_play_mem(0, 0,	0);
					buttons[0] = 0;
					return	button_1;
				}
			}
		
			if (buttons[1] == 0)
			{
				if (pin_test (PIN_BUTTON_3) == 0) buttons[1] = 1;
				else buttons[1] = 0;
			}
			else
			{
				if (pin_test (PIN_BUTTON_3) == 1) 
				{
					dprintf ("Send scene stop cmd");
					//player_stop_mem();
					buttons[1] = 0;
					return button_3;
				}
			}

			if (buttons[2] == 0)
			{
				if (pin_test (PIN_BUTTON_4) == 0) buttons[2] = 1;
				else buttons[2] = 0;
			}
			else
			{
				if (pin_test (PIN_BUTTON_4) == 1) 
				{
					dprintf ("Send scene stop cmd");
					//player_stop_mem();
					buttons[2] = 0;
					return button_4;
				}
			}
			if (buttons[3] == 0)
			{
				if (pin_test (PIN_BUTTON_2) == 0) buttons[3] = 1;
				else buttons[3] = 0;
			}
			else
			{
				if (pin_test (PIN_BUTTON_2) == 1) 
				{
					dprintf ("Send scene stop cmd");
					//player_stop_mem();
					buttons[3] = 0;
					return button_2;
				}
			}

		}
		return	null;
	}
