#ifndef _DELAY_H_
#define _DELAY_H_

#include "core_cm3.h"
//=============================================================================
// Auto generated define from  skript /Keil to Other IDE converter/ 
//#define __NOP() __ASM volatile ("nop")


static __inline void delay_8cycles(uint32_t x)
{
/*
	subs r0
	bcs  exit

	mrs r0, primask
exit:
	bx lr

*/

	while (x--) // 4  SUBS + BCS + PIPLINE_RELOAD
	{
		__NOP(); //	5
		__NOP(); //	6
		__NOP(); //	7
		__NOP(); //	8
	}
}

//=============================================================================
static __inline void delay_cycles(uint32_t x)
{
	uint32_t rem = x&7; // x%8

	if      (rem==1)  {__NOP();}
	else if (rem==2)  {__NOP();__NOP();}
	else if (rem==3)  {__NOP();__NOP();__NOP();}
	else if (rem==4)  {__NOP();__NOP();__NOP();__NOP();}
	else if (rem==5)  {__NOP();__NOP();__NOP();__NOP();__NOP();}
	else if (rem==6)  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}
	else if (rem==7)  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}

	if ((x/=8))       delay_8cycles(x);
}

//=============================================================================
static __inline void delay_ns  (double ns)    {delay_cycles(ns/((double)1e9/(double)F_CPU) + 0.5);}
void                 delay_us  (uint32_t us);
void                 delay_ms  (uint32_t ms);
void                 delay_sec (uint32_t sec);

#endif
