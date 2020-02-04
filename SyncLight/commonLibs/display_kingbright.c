//#include "display_kingbright.h"
#include "include.h"

//=======================================================================
void Display_init( Display *structure, uint8_t _number_of_symb)
{
	structure->number_of_symb = _number_of_symb;
	//structure->digs = new dig[number_of_symb];
	structure->digs[0].synbol = one;
	structure->digs[1].synbol = two;
	structure->digs[2].synbol = three;
	structure->digs[3].synbol = four;
	
	structure->delay = systick_timer.tics;
	structure->display_symbol = 0;
}
//=======================================================================
void init_symbol( Display *structure, GPIO_TypeDef *aGPIO, uint16_t aPIN,
		GPIO_TypeDef *bGPIO, uint16_t bPIN,
		GPIO_TypeDef *cGPIO, uint16_t cPIN,
		GPIO_TypeDef *dGPIO, uint16_t dPIN,
		GPIO_TypeDef *eGPIO, uint16_t ePIN,
		GPIO_TypeDef *fGPIO, uint16_t fPIN,
		GPIO_TypeDef *gGPIO, uint16_t gPIN)
{
	structure->disp_symbol.aPIN = aPIN;
	structure->disp_symbol.aPORT = aGPIO;
	
	structure->disp_symbol.bPIN = bPIN;
	structure->disp_symbol.bPORT = bGPIO;
	
	structure->disp_symbol.cPIN = cPIN;
	structure->disp_symbol.cPORT = cGPIO;
	
	structure->disp_symbol.dPIN = dPIN;
	structure->disp_symbol.dPORT = dGPIO;
	
	structure->disp_symbol.ePIN = ePIN;
	structure->disp_symbol.ePORT = eGPIO;
	
	structure->disp_symbol.fPIN = fPIN;
	structure->disp_symbol.fPORT = fGPIO;
	
	structure->disp_symbol.gPIN = gPIN;
	structure->disp_symbol.gPORT = gGPIO;
	
}
//=======================================================================
void init_dig( Display *structure, GPIO_TypeDef *digGPIO, uint16_t digPIN,int suymbol_number )
{
	structure->digs[suymbol_number].digPORT = digGPIO;
	structure->digs[suymbol_number].digPIN  = digPIN;
}
//=======================================================================
void symbol_enable( Display *structure,  uint8_t _symbol)
{
	if(_symbol&0x40)
	structure->disp_symbol.aPORT->BSRR |= pin_bssr(structure->disp_symbol.aPIN, true);
	else structure->disp_symbol.aPORT->BSRR |= pin_bssr(structure->disp_symbol.aPIN, false);
	
	if(_symbol&0x20)
	structure->disp_symbol.bPORT->BSRR |= pin_bssr(structure->disp_symbol.bPIN, true);
	else structure->disp_symbol.bPORT->BSRR |= pin_bssr(structure->disp_symbol.bPIN, false);
	
	if(_symbol&0x10)
	structure->disp_symbol.cPORT->BSRR |= pin_bssr(structure->disp_symbol.cPIN, true);
	else structure->disp_symbol.cPORT->BSRR |= pin_bssr(structure->disp_symbol.cPIN, false);
	
	if(_symbol&0x8)
	structure->disp_symbol.dPORT->BSRR |= pin_bssr(structure->disp_symbol.dPIN, true);
	else structure->disp_symbol.dPORT->BSRR |= pin_bssr(structure->disp_symbol.dPIN, false);
	
	if(_symbol&0x4)
	structure->disp_symbol.ePORT->BSRR |= pin_bssr(structure->disp_symbol.ePIN, true);
	else structure->disp_symbol.ePORT->BSRR |= pin_bssr(structure->disp_symbol.ePIN, false);
	
	if(_symbol&0x2)
	structure->disp_symbol.fPORT->BSRR |= pin_bssr(structure->disp_symbol.fPIN, true);
	else structure->disp_symbol.fPORT->BSRR |= pin_bssr(structure->disp_symbol.fPIN, false);
	
	if(_symbol&0x1)
	structure->disp_symbol.gPORT->BSRR |= pin_bssr(structure->disp_symbol.gPIN, true);
	else structure->disp_symbol.gPORT->BSRR |= pin_bssr(structure->disp_symbol.gPIN, false);
}
//=======================================================================
uint32_t pin_bssr(uint8_t _pin, uint8_t _enable)
{
	if(_enable >0)
	{
	switch(_pin)
	{
		case 0:return 	GPIO_BSRR_BS0;
		case 1:return 	GPIO_BSRR_BS1;
		case 2:return 	GPIO_BSRR_BS2;
		case 3:return 	GPIO_BSRR_BS3;
		case 4:return 	GPIO_BSRR_BS4;
		case 5:return 	GPIO_BSRR_BS5;
		case 6:return 	GPIO_BSRR_BS6;
		case 7:return 	GPIO_BSRR_BS7;
		case 8:return 	GPIO_BSRR_BS8;
		case 9:return 	GPIO_BSRR_BS9;
		case 10:return	GPIO_BSRR_BS10;
		case 11:return	GPIO_BSRR_BS11;
		case 12:return	GPIO_BSRR_BS12;
		case 13:return	GPIO_BSRR_BS13;
		case 14:return	GPIO_BSRR_BS14;
		case 15:return	GPIO_BSRR_BS15;
		default:break;
	}
}
	else{
		switch(_pin)
	{
		case 0:return 	GPIO_BSRR_BR0;
		case 1:return 	GPIO_BSRR_BR1;
		case 2:return 	GPIO_BSRR_BR2;
		case 3:return 	GPIO_BSRR_BR3;
		case 4:return 	GPIO_BSRR_BR4;
		case 5:return 	GPIO_BSRR_BR5;
		case 6:return 	GPIO_BSRR_BR6;
		case 7:return 	GPIO_BSRR_BR7;
		case 8:return 	GPIO_BSRR_BR8;
		case 9:return 	GPIO_BSRR_BR9;
		case 10:return	GPIO_BSRR_BR10;
		case 11:return	GPIO_BSRR_BR11;
		case 12:return	GPIO_BSRR_BR12;
		case 13:return	GPIO_BSRR_BR13;
		case 14:return	GPIO_BSRR_BR14;
		case 15:return	GPIO_BSRR_BR15;
		default:break;
	}
	}
	return 0;
}
//=======================================================================
void dislay_Handler( Display *structure)
{
	for(uint8_t i = 0; i<structure->number_of_symb; i++)
	{
		if(i==structure->display_symbol)
		{
			symbol_enable( structure, structure->digs[i].synbol);
			structure->digs[i].digPORT->BSRR |= pin_bssr(structure->digs[i].digPIN, ENABLE);
		}
		else 
		{
			structure->digs[i].digPORT->BSRR |= pin_bssr(structure->digs[i].digPIN, DISABLE);
		}
	}
		structure->display_symbol++;
		if(structure->display_symbol > structure->number_of_symb)  structure->display_symbol = 0;
}
//=======================================================================
void set_string( Display *structure, char new_symb[], int number)
{
	for(uint8_t i = 0;i <number; i++)
	{
		structure->digs[i].synbol = matching_symbol(new_symb[i]);
	}
}
//=======================================================================
uint8_t matching_symbol(char _symbol)
{
	switch(_symbol)
	{
		case '0':return zero;
		case '1':return one;
		case '2':return two;
		case '3':return three;
		case '4':return four;
		case '5':return five;
		case '6':return six;
		case '7':return seven;
		case '8':return eight;
		case '9':return nine;
		
		case ' ':return space;
		case 'A':return A;
		case 'B':return B;
		case 'b':return b;
		case 'C':return C;
		case 'c':return c;
		case 'D':return D;
		case 'd':return d;
		case 'E':return E;
		case 'F':return F;
		case 'H':return H;
		case 'h':return h;
		case 'R':return R;
		case 'r':return R;
		case 'G':return G;
		case 'g':return g;
		case 'N':return N;
		case 'L':return L;
		case 'U':return U;
		case 'I':return I;
		case 'M':return M;
		case 'S':return S;
		case 'T':return T;
		case 'P':return P;
		default:return 0;
	}
}






