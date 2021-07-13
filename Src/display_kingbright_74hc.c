#include "display_kingbright_74hc.h"

symbol disp_symbols[SYMBOLS_NUMBER];
ch74_input ch74;
//=======================================================================
void display_init_74hc(GPIO_TypeDef *aGPIO, uint8_t a_pin, GPIO_TypeDef *bGPIO, uint8_t b_pin, GPIO_TypeDef *cGPIO, uint8_t c_pin, GPIO_TypeDef *dGPIO, uint8_t d_pin)
{
	ch74.port_a.digPORT = aGPIO;
	ch74.port_a.digPIN = a_pin;

	ch74.port_b.digPORT = bGPIO;
	ch74.port_b.digPIN = b_pin;

	ch74.port_c.digPORT = cGPIO;
	ch74.port_c.digPIN = c_pin;

	ch74.port_d.digPORT = dGPIO;
	ch74.port_d.digPIN = d_pin;
}
//=======================================================================
void display_init_symbol(GPIO_TypeDef *aGPIO, uint16_t aPIN, GPIO_TypeDef *bGPIO, uint16_t bPIN, GPIO_TypeDef *cGPIO, uint16_t cPIN, GPIO_TypeDef *dGPIO, uint16_t dPIN)
{
	disp_symbols[0].gpio.digPIN = aPIN;
	disp_symbols[0].gpio.digPORT = aGPIO;

	disp_symbols[1].gpio.digPIN = bPIN;
	disp_symbols[1].gpio.digPORT = bGPIO;

	disp_symbols[2].gpio.digPIN = cPIN;
	disp_symbols[2].gpio.digPORT = cGPIO;

	disp_symbols[3].gpio.digPIN = dPIN;
	disp_symbols[3].gpio.digPORT = dGPIO;
}
//=======================================================================
uint32_t pin_bssr(uint16_t GPIO_Pin, uint8_t _enable)
{
	if (_enable > 0)
	{
		switch (GPIO_Pin)
		{
		case 0:
			return GPIO_BSRR_BS0;
		case 1:
			return GPIO_BSRR_BS1;
		case 2:
			return GPIO_BSRR_BS2;
		case 3:
			return GPIO_BSRR_BS3;
		case 4:
			return GPIO_BSRR_BS4;
		case 5:
			return GPIO_BSRR_BS5;
		case 6:
			return GPIO_BSRR_BS6;
		case 7:
			return GPIO_BSRR_BS7;
		case 8:
			return GPIO_BSRR_BS8;
		case 9:
			return GPIO_BSRR_BS9;
		case 10:
			return GPIO_BSRR_BS10;
		case 11:
			return GPIO_BSRR_BS11;
		case 12:
			return GPIO_BSRR_BS12;
		case 13:
			return GPIO_BSRR_BS13;
		case 14:
			return GPIO_BSRR_BS14;
		case 15:
			return GPIO_BSRR_BS15;
		default:
			break;
		}
	}
	else
	{
		switch (GPIO_Pin)
		{
		case 0:
			return GPIO_BSRR_BR0;
		case 1:
			return GPIO_BSRR_BR1;
		case 2:
			return GPIO_BSRR_BR2;
		case 3:
			return GPIO_BSRR_BR3;
		case 4:
			return GPIO_BSRR_BR4;
		case 5:
			return GPIO_BSRR_BR5;
		case 6:
			return GPIO_BSRR_BR6;
		case 7:
			return GPIO_BSRR_BR7;
		case 8:
			return GPIO_BSRR_BR8;
		case 9:
			return GPIO_BSRR_BR9;
		case 10:
			return GPIO_BSRR_BR10;
		case 11:
			return GPIO_BSRR_BR11;
		case 12:
			return GPIO_BSRR_BR12;
		case 13:
			return GPIO_BSRR_BR13;
		case 14:
			return GPIO_BSRR_BR14;
		case 15:
			return GPIO_BSRR_BR15;
		default:
			break;
		}
	}
	return 0;
}
//=======================================================================
void matching_symbol(ch74_input *ch74, uint8_t _symbol)
{
	ch74->port_a.digPORT->BSRR |= pin_bssr(ch74->port_a.digPIN, _symbol & 0x01);

	ch74->port_b.digPORT->BSRR |= pin_bssr(ch74->port_b.digPIN, (_symbol >> 1) & 0x01);

	ch74->port_c.digPORT->BSRR |= pin_bssr(ch74->port_c.digPIN, (_symbol >> 2) & 0x01);

	ch74->port_d.digPORT->BSRR |= pin_bssr(ch74->port_d.digPIN, (_symbol >> 3) & 0x01);
}
//=======================================================================
void display_set_string(char *new_symb)
{
	for (uint8_t i = 0; i < sizeof(new_symb); i++)
	{
		disp_symbols[i].synbol = new_symb[i];
	}
}
//=======================================================================
void dislay_Handler(void)
{
	static uint32_t LastUpdateTime = 0;

	if ((systick_timer.tics - LastUpdateTime) > 5)
	{
		static int symbol_number = 0;
		for (uint8_t i = 0; i < SYMBOLS_NUMBER; i++)
		{
			if (i == symbol_number)
			{
				matching_symbol(&ch74, disp_symbols[i].synbol);

				disp_symbols[i].gpio.digPORT->BSRR |= pin_bssr(disp_symbols[i].gpio.digPIN, ENABLE);
			}
			else
			{
				disp_symbols[i].gpio.digPORT->BSRR |= pin_bssr(disp_symbols[i].gpio.digPIN, DISABLE);
			}
		}
		symbol_number++;
		if (symbol_number > SYMBOLS_NUMBER)
		{
            symbol_number = 0;
        }
		LastUpdateTime = systick_timer.tics;
	}
}
//=======================================================================
