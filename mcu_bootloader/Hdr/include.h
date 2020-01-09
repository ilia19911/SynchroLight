#ifndef _INCLUDE_H_
#define _INCLUDE_H_

/* ======================================================================	*/
/* Старт проекта: 05.09.2019																							*/
/* Автор: Харитоненко Владимир																						*/
/* ======================================================================	*/

// Глобальные дефайны
#define F_CPU								72000000
#define HSE_VALUE						16000000

#define VERSION_MAJOR					1
#define VERSION_MINOR					2

// Заголовки стандартной Сишной либлиотеки 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Заголовок для проца
#include "stm32f10x.h"

// Заголовки краеугольных камней unilib'а
#include "global_macro.h"
#include "static_assert.h"
#include "atomic.h"
#include "rcc.h"
#include "pin_macro.h"

// Заголовки библиотечных модулей проекта
#include "fifo.h"
#include "uart.h"
#include "systick_timer.h"
#include "endianes.h"
#include "delay.h"
#include "wdt.h"
#include "crc32.h"
#include "crc8.h"
#include "sx1276_transc.h"

// Заголовки модулей проекта
#include "bootloader.h"

// Заголовки конфигурации аппаратной части
#ifdef PLATE_MASTER
#include "low_level_master.h"
#elif defined PLATE_SLAVE
#include "low_level_slave.h"
#else
#error Please select type of board - PLATE_MASTER or PLATE_SLAVE
#endif

#include "uart_conf.h"

// Заголовок глобальных переменных
#include "var.h"

#endif /* _INCLUDE_H_ */
