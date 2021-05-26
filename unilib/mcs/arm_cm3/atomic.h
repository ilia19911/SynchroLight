/*
 *  atomic.h rev.1.0.0 2010-06-28
 *
 *  Cortex-m3 KEIL atomic module.
 *
 *  Contributors:
 *                Ivanov Anton (c) www.automatix.ru
 *
 */


#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#ifdef __cplusplus
  extern "C" {
#endif

//#pragma diag_suppress 667
//#pragma diag_error 667
//#pragma diag_remark 667

//=============================================================================
//************ Auto generated Func from  skript /Keil to Other IDE converter/************ 
 static __inline  uint32_t get_interrupt_state(void) 
{ 
	__asm volatile( 
	"mrs r0, primask" 
	"bx lr"
	);
}
//static __inline __asm uint32_t get_interrupt_state(void)
//{
//	mrs r0, primask
//	bx lr
//}

//=============================================================================
//************ Auto generated Func from  skript /Keil to Other IDE converter/************ 
static __inline void set_interrupt_state(uint32_t status)
{
	__asm volatile(
	"msr primask, r0"
	"bx lr"
	);
}
//static __inline __asm void set_interrupt_state(uint32_t status)
//{
//	msr primask, r0
//	bx lr
//}


#define ENTER_CRITICAL_SECTION() do {uint32_t sreg_temp = __get_PRIMASK(); __disable_irq(); 
#define LEAVE_CRITICAL_SECTION() __set_PRIMASK(sreg_temp);} while (0) 



#ifdef __cplusplus
  }
#endif

#endif
