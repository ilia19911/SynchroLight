#ifndef _INCLUDE_H_
#define _INCLUDE_H_

/* ======================================================================	*/
/* Старт проекта: 05.09.2016																							*/
/* Автор: Харитоненко Владимир																						*/
/* ======================================================================	*/

// Глобальные дефайны
#define F_CPU							72000000
#define VERSION_MINOR			2
#define VERSION_MAJOR			0
#define VERSION_LITERAL		"a"

//#define __nop __NOP 


// Заголовки стандартной Сишной либлиотеки 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
#include "crc8.h"
#include "crc32.h"
#include "sx1276_transc.h"
#include "adclib.h"
#include "randomizer.h"
#include "spi_bitbang.h"
#include "dprintf.h"
#include "at25df.h"
#include "at25db321_fileStorage.h"
#include "uni_flash.h"
#include "uni_eeprom.h"



// Заголовки модулей проекта
#include "rf_parser.h"
#ifdef SLAVE
// Заголовки конфигурации аппаратной части
#include "low_level0_slave.h"
#include "ff.h"
#include "fileTransfer.h"
#include "display_kingbright.h"
#include "led_maping.h"
#include "player.h"
#include "ws2812b.h"
#include "scen_memory.h"
#include "effect_processor.h"
// Заголовок глобальных переменных
#include "var_slave.h"
#endif
#ifdef MASTER
// Заголовки конфигурации аппаратной части
#ifdef MASTER_V2
#include "low_level0_master_v2.h"
#else
#include "low_level0_master.h"
#endif
#include "buttons.h"
#include "ff.h"
#include "fileTransfer.h"
#include "display_kingbright.h"
#include "display_menu.h"
//#include "led_maping.h"
#include "player.h"
#include "scen_memory.h"
// Заголовок глобальных переменных
#include "var_master.h"
//#include "effect_processor.h"
#endif


#endif /* _INCLUDE_H_ */
