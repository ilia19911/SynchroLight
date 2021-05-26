#ifndef _DPRINTF_CONF_H_
#define _DPRINTF_CONF_H_

/* ======================================================================	*/
/* Настройка дебаг принтф																									*/
/* По умолчанию (нет дефайна ITM_DEBUG) - все выводится на UART						*/
/* Для этого: в дефайне dputchar(x) определить uart_putc(&uart_debug,(x))	*/
/* Где uart_debug - *uart_t библиотеки unilib															*/
/* Если поставить ITM_DEBUG - то отладочная информация будет отправляться	*/
/* в trace 0 (в Keil надо будет включить trace mode)											*/
/* ======================================================================	*/
struct __FILE { int handle;};
FILE __stdout;
FILE __stdin;
#define debug_stream		&__stdout

#include "include.h"

#define debug_stream   &__stdout

#ifndef ITM_DEBUG
#	define dputchar(x)  uart_putc(&uart_debug,(x))
#else
#	define dputchar(x)  ITM_SendChar((x)) // inside core_cm3.h (cmsis)
#endif

#endif
