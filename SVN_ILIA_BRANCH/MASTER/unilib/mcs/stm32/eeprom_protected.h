/*
 * stm32f10xxx or stm32f10xxx eeprom on flash memory emulation.
 *
 */



#ifndef _EEPROM_H_
#define _EEPROM_H_



/*
*	For STM32F40x use this table and look at flash_iap.
*	from 0 to 3 - 16kB, 0 - 64kB, 0 to 6 - 128 - kbyte, if you change 
*	page number page flash/configure flash/target/on-chip IROM1 and IROM2
*	
*	For stm32f1xx diap from 0 to 127
*/

#define EEPROM_MAX_SIZE    FLASH_PAGE_SIZE
#define EEPROM_FLASH_BASE  FLASH_PAGE_ADDRES(127) 

void eeprom_protected_init(void);
int  eeprom_protected_sync(void);

#endif
