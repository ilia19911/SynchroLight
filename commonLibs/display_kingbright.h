#ifndef __display_kingbright_H
#define __display_kingbright_H

#include "include.h"

//#define PIN_BSSR1(a, b)  a##b
//#define PIN_BSSR(a) PIN_BSSR1(GPIO_BSRR_BS, a)
#define FALSE 0x00
#define TRUE 0x01

#define SYMBOLS_NUMBER 4

//	void clear(void);
//	void start(void);
//	void stop(void);

typedef struct {
	GPIO_TypeDef  *digPORT;
	int digPIN;
}dig;
		
	
typedef struct {
	dig gpio;
	uint8_t synbol;
}symbol;

typedef struct{
	dig port_a;
	dig port_b;
	dig port_c;
	dig port_d;
}ch74_input;
		
void display_init_74hc(GPIO_TypeDef *aGPIO, uint8_t a_pin,GPIO_TypeDef *bGPIO, uint8_t b_pin, GPIO_TypeDef *cGPIO, uint8_t c_pin,GPIO_TypeDef *dGPIO,uint8_t d_pin);

void display_init_symbol(GPIO_TypeDef *aGPIO, uint16_t aPIN, GPIO_TypeDef *bGPIO, uint16_t bPIN, GPIO_TypeDef *cGPIO, uint16_t cPIN, GPIO_TypeDef *dGPIO, uint16_t dPIN);

void  dislay_Handler(void);

void display_set_symbol(char new_symb, int number);
void display_get_symbol(char new_symb, int number);
void display_set_string(char *new_symb);
void display_get_string(char new_symb[], int number);

#endif
	
	
