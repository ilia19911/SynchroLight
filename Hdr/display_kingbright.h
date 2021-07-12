#ifndef __display_kingbright_H
#define __display_kingbright_H

#include "include.h"

#define PIN_BSSR1(a, b)  a##b
#define PIN_BSSR(a) PIN_BSSR1(GPIO_BSRR_BS, a)

#define true 1
#define false 0

enum{
    zero    =   0x7E,
    one     =   0x30,
    two     =   0x6D,
    three   =   0x79,
    four    =   0x33,
    five    =   0x5B,
    six     =   0x5F,
    seven   =   0x70,
    eight   =   0x7F,
    nine    =   0x7B,
    space   =   0x00,
    A       =   0x77,
    B       =   0x1F,
    b       =   0x1f,
    C       =   0x4E,
    c       =   0x0d,
    D       =   0x3D,
    d       =   0x3d,
    E       =   0x4F,
    F       =   0x47,
    H       =   0x37,
    h       =   0x17,
    R       =   0x05,
    G       =   0x5E,
    g       =   0x7b,
    N       =   0x15,
    L       =   0x0E,
    U       =   0x3E,
    I       =   0x30,
    M       =   0x76,
    S       =   0x5B,
    T       =   0x0F,
    P       =   0x67
};

typedef struct {
	GPIO_TypeDef  *aPORT;
	int aPIN;
	GPIO_TypeDef  *bPORT;
	int bPIN;
	GPIO_TypeDef  *cPORT;
	int cPIN;
	GPIO_TypeDef  *dPORT;
	int dPIN;
	GPIO_TypeDef  *ePORT;
	int ePIN;
	GPIO_TypeDef  *fPORT;
	int fPIN;
	GPIO_TypeDef  *gPORT;
	int gPIN;
}symbol;

typedef struct {
	GPIO_TypeDef  *digPORT;
	int digPIN;
	uint8_t synbol;
}dig;

typedef struct{
	int a ;
} dot;


typedef struct 
{
    symbol disp_symbol;
    dig digs[4];
    dot *disp_dots;
    
    int number_of_symb;
    int display_symbol;
    
    uint32_t delay;

}Display;



void display_init(uint8_t _number_of_symb);

void        display_init_symbol( GPIO_TypeDef *aGPIO, uint16_t aPIN,
	GPIO_TypeDef *bGPIO, uint16_t bPIN,
	GPIO_TypeDef *cGPIO, uint16_t cPIN,
	GPIO_TypeDef *dGPIO, uint16_t dPIN,
	GPIO_TypeDef *eGPIO, uint16_t ePIN,
	GPIO_TypeDef *fGPIO, uint16_t fPIN,
	GPIO_TypeDef *gGPIO, uint16_t gPIN
	);
void        display_init_dig(GPIO_TypeDef *digGPIO, uint16_t digPIN,int suymbol_number );

void        display_set_symbol(Display *structure, char new_symb, int number);
void        display_get_symbol(Display *structure, char new_symb, int number);
void        display_set_string(char *new_symb);
void        display_get_string(Display *structure, char new_symb[], int number);
//void        set_dot(Display *structure, int number, uint8_t value);
//void        clear();
//void        start();
//void        stop();
	
void        dislay_Handler( void);
void        display_symbol_enable( Display *structure, uint8_t _symbol);
uint32_t    pin_bssr( uint8_t _pin, uint8_t _enable);
uint8_t     matching_symbol(char _symbol);



#endif
