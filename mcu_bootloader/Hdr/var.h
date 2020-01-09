#ifndef _VAR_H_
#define _VAR_H_

/* ======================================================================	*/
/* Тут описываются ВСЕ глобальный переменные проекта.											*/
/* Только тут.																														*/
/* ======================================================================	*/
#define BOOTLOADER_VERSION_MAJOR									1
#define BOOTLOADER_VERSION_MINOR									1

#define RF_CLOCK_FREQUENCY												32000000

#ifdef MAIN

#ifdef BOOTLOADER_MODULE_SX1276
sx1276_LoRa_Modem T;
uint16_t sx1276address;
#endif /* BOOTLOADER_MODULE_SX1276 */

#else /* MAIN */

#include "include.h"

#ifdef BOOTLOADER_MODULE_SX1276
extern sx1276_LoRa_Modem T;
extern uint16_t sx1276address;
#endif /* BOOTLOADER_MODULE_SX1276 */

#endif /* MAIN */

#endif /* _VAR_H_ */
