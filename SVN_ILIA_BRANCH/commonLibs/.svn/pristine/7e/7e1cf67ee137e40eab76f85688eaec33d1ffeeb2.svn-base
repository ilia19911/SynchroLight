/*
 * stm32f10xxx flash memory in application programming module (according to pm0042).
 *
 * Notes:
 *     During a write operation to the Flash memory, any attempt to read the Flash memory will stall the bus.
 * The read operation will proceed correctly once the write operation has completed.
 * This means that code or data fetches cannot be made while a write/erase operation is ongoing.
 * For write and erase operations on the Flash memory (write/erase), the internal RC oscillator (HSI) must be ON.
 * In the low-power modes, all Flash memory accesses are aborted.
 *
 */

#ifndef _UNI_FLASH_H_
#define _UNI_FLASH_H_

#if F1_CHECK
	
	#define FLASH_START              0x08000000
	
	#if  defined(STM32F10X_LD) || defined(STM32F10X_LD_VL)
	
		#define FLASH_SECTORS_CNT  32
		#define FLASH_PAGE_SIZE    1024vvc
		#define FLASH_SIZE         (FLASH_SECTORS_CNT*FLASH_PAGE_SIZE)  // 32k max
	
	#elif  defined(STM32F10X_MD) || defined(STM32F10X_MD_VL)

		#define FLASH_SECTORS_CNT  128
		#define FLASH_PAGE_SIZE    1024
		#define FLASH_SIZE         (FLASH_SECTORS_CNT*FLASH_PAGE_SIZE) // 128k max
				
	#elif  defined(STM32F10X_CL)
	
		#define FLASH_SECTORS_CNT  128
		#define FLASH_PAGE_SIZE    2048
		#define FLASH_SIZE         (FLASH_SECTORS_CNT*FLASH_PAGE_SIZE) // 256k max
			
	#elif  defined(STM32F10X_HD)
	
		#define FLASH_SECTORS_CNT  256
		#define FLASH_PAGE_SIZE    2048
		#define FLASH_SIZE         (FLASH_SECTORS_CNT*FLASH_PAGE_SIZE) // 512k max
			
	#else
	
		#error WRONG MCU_LINE!!!
	
	#endif
	
#elif F37_CHECK
	
	#define FLASH_START        0x08000000
	
	#define FLASH_SECTORS_CNT  128
	#define FLASH_PAGE_SIZE    2048
	#define FLASH_SIZE         (FLASH_SECTORS_CNT*FLASH_PAGE_SIZE) // 256k max
		
#else //#elif defined (STM32F40_41xxx)

	#define FLASH_START        0x08000000
	
	#define FLASH_SECTORS_CNT  			12
	
	#define FLASH_SECTORS_CNT_0_3 	4
	#define FLASH_PAGE_SIZE_0_3   	(16*1024)
	#define FLASH_SECTORS_CNT_4 		1
	#define FLASH_PAGE_SIZE_4   		(64*1024)
	#define FLASH_SECTORS_CNT_5_11 	7
	#define FLASH_PAGE_SIZE_5_11   	(128*1024)
	#define FLASH_SIZE	((FLASH_SECTORS_CNT_0_3 * FLASH_PAGE_SIZE_0_3) + (FLASH_SECTORS_CNT_4 * FLASH_PAGE_SIZE_4) + (FLASH_SECTORS_CNT_5_11*FLASH_PAGE_SIZE_5_11))
				
#endif

//=============================================================================
ErrorStatus uni_flash_write_block(const uint8_t* dst, const uint8_t* src, size_t len);
ErrorStatus uni_flash_erase_page(uint32_t page_address_start);
ErrorStatus uni_flash_check_page_number(uint32_t page_number);
uint32_t uni_flash_get_page_address_start(uint32_t page_number);
uint32_t uni_flash_get_page_address_end(uint32_t page_number);
void uni_flash_test(void);

#endif
