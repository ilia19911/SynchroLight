#include "include.h"

#if F1_CHECK || F37_CHECK
	
	#pragma pack(push, 4)
	typedef struct
	{
		volatile uint32_t ACR;       // BASE + 0x00
		volatile uint32_t KEYR;      // BASE + 0x04
		volatile uint32_t OPTKEYR;   // BASE + 0x08
		volatile uint32_t SR;        // BASE + 0x0C
		volatile uint32_t CR;        // BASE + 0x10
		volatile uint32_t AR;        // BASE + 0x14
						 uint32_t _dummy;    // BASE + 0x18
		volatile uint32_t OBR;       // BASE + 0x1C
		volatile uint32_t WRPR;      // BASE + 0x20
	} fpec_t;
	#pragma pack(pop)
	
	#define FPEC_BASE                0x40022000UL
	
	enum // Flash control register bits
	{
		FPEC_CR_PG     = (1<<0),
		FPEC_CR_PER    = (1<<1),
		FPEC_CR_MER    = (1<<2),

		FPEC_CR_OPTPG  = (1<<4),
		FPEC_CR_OPTER  = (1<<5),
		FPEC_CR_STRT   = (1<<6),
		FPEC_CR_LOCK   = (1<<7),

		FPEC_CR_OPTWRE = (1<<9),
		FPEC_CR_ERRIE  = (1<<10),

		FPEC_CR_EOPIE  = (1<<12)
	};

	enum // Flash status register bits
	{
		FPEC_SR_BSY      = (1<<0),  // FLASH Busy flag

		FPEC_SR_PGERR    = (1<<2),  // FLASH Program error flag

		FPEC_SR_WRPRTERR = (1<<4),  // FLASH Write protected error flag
		FPEC_SR_EOP      = (1<<5)   // FLASH End of Operation flag
	};
	
#else //F4
	#pragma pack(push, 4)
	typedef struct
	{
		volatile uint32_t ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
		volatile uint32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
		volatile uint32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
		volatile uint32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
		volatile uint32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
		volatile uint32_t OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
		volatile uint32_t OPTCR1;   /*!< FLASH option control register 1, Address offset: 0x18 */
	} fpec_t;
	#pragma pack(pop)

	#define FPEC_CR_PSIZE_MASK              ((uint32_t)0xFFFFFCFF)
	#define FPEC_SECTOR_MASK				((uint32_t)0xFFFFFF07)
	#define FPEC_CR_LOCK   					((uint32_t)0x80000000)
	
	enum // Flash control register bits
	{
		FPEC_CR_PG     = (1<<0),
		FPEC_CR_SER    = (1<<1),
		FPEC_CR_MER    = (1<<2),
		
		FPEC_PSIZE_HALF_WORD = (1<<8),		
		
		FPEC_CR_STRT   = (1<<16),
		FPEC_CR_EOPIE  = (1<<24)			
	};

	enum // Flash status register bits
	{
		FPEC_SR_BSY      = (1<<16),  // FLASH Busy flag

		FPEC_SR_PGERR    = (1<<6),	//FLASH Write protected error flag

		FPEC_SR_WRPRTERR = (1<<4),  // FLASH Write protected error flag
		FPEC_SR_EOP      = (1<<0)   // FLASH End of Operation flag
	};
	
	#define FPEC_BASE                0x40023C00UL
	
#endif

#define FPEC	((fpec_t*)FPEC_BASE)

//=============================================================================
ErrorStatus uni_flash_start_programming(void)
{		
	static const uint32_t FPEC_KEY1 = 0x45670123;
	static const uint32_t FPEC_KEY2 = 0xCDEF89AB;
	
	FPEC->KEYR = FPEC_KEY1;
	FPEC->KEYR = FPEC_KEY2;
	
	while (FPEC->SR & FPEC_SR_BSY) {;}
		
	FPEC->SR = FPEC_SR_EOP;
		
#if (F1_CHECK || F37_CHECK)	
	
#else	
	FPEC->CR &= FPEC_CR_PSIZE_MASK;
	FPEC->CR |= FPEC_PSIZE_HALF_WORD;
#endif
		
	FPEC->CR |= FPEC_CR_PG;

	if(FPEC->CR & FPEC_CR_LOCK)
	{
		return ERROR;
	}
	else
	{
		return SUCCESS;
	}
}

//=============================================================================
void uni_flash_stop_programming(void)
{	
	FPEC->CR &= (~FPEC_CR_PG);
			
	FPEC->CR |= FPEC_CR_LOCK;
}

//=============================================================================
ErrorStatus uni_flash_write_block(const uint8_t* dst, const uint8_t* src, size_t len)
{
	ErrorStatus complete_fl;
		
	for (const uint8_t* end = dst + len; dst < end; dst += 2)
	{
		uint16_t x;

		if ((uint32_t)dst & 1)  {x = ~0x00; dst--;}    // odd flash address - step back
		else                    {x = ~0xFF | *src++;}

		if (dst < end)  x = (x & 0xFF) | (*src++ << 8);
		
		ENTER_CRITICAL_SECTION();
		
		complete_fl = uni_flash_start_programming();
		
		if (complete_fl == SET)
		{
			*(volatile uint16_t*)dst = x;
		
			while ((!(FPEC->SR & FPEC_SR_EOP)) && (FPEC->SR & FPEC_SR_BSY)) {};
			
			FPEC->SR = FPEC_SR_EOP;
		}
		
		uni_flash_stop_programming();
		
		LEAVE_CRITICAL_SECTION();
		
		if (*(volatile uint16_t*)dst != x || complete_fl == ERROR)
		{
			complete_fl = ERROR;
			break;
		}
	}	
	
	return (complete_fl);
}

//=============================================================================
ErrorStatus uni_flash_erase_page(uint32_t page_address_start) // page_address not page_number
{
	static const uint32_t FPEC_KEY1 = 0x45670123;
	static const uint32_t FPEC_KEY2 = 0xCDEF89AB;
	
	int res;

	ENTER_CRITICAL_SECTION();
	{
#if F1_CHECK || F37_CHECK
		
		FPEC->KEYR = FPEC_KEY1;                // Authorize the FPEC Access 
		FPEC->KEYR = FPEC_KEY2;

		while (FPEC->SR & FPEC_SR_BSY) {;}

		FPEC->SR = FPEC_SR_EOP | FPEC_SR_WRPRTERR | FPEC_SR_PGERR; // clr bits by writing 1
		FPEC->CR = FPEC_CR_PER;                // page erase operation w/o fpec interrupts
		FPEC->AR = page_address_start;               // page address
		FPEC->CR = FPEC_CR_STRT | FPEC_CR_PER; // start operation	
		FPEC->SR;							   // according to errata: must be dummy cycle before polling BSY after START operation

		while (FPEC->SR & FPEC_SR_BSY) {;}

		res = ((FPEC->SR & FPEC_SR_EOP) != 0); // EOP is asserted at the end of each successful program or erase operation

		FPEC->CR = FPEC_CR_LOCK;               // Set the Lock Bit to lock the FPEC and the FCR

#else
		
		FPEC->KEYR = FPEC_KEY1;                // Authorize the FPEC Access 
		FPEC->KEYR = FPEC_KEY2;

		while (FPEC->SR & FPEC_SR_BSY) {;}
		
		FPEC->CR &= FPEC_CR_PSIZE_MASK;
		FPEC->CR |= FREC_PSIZE_HALF_WORD; 			
	  FPEC->CR &= FPEC_SECTOR_MASK;
			
	  FPEC->CR |= FPEC_CR_SER | page_address_start;
		FPEC->CR |= FPEC_CR_STRT;
		
		while (FPEC->SR & FPEC_SR_BSY) {;}
			
		res = ((FPEC->SR & FPEC_SR_EOP) != 0); // EOP is asserted at the end of each successful program or erase operation
		
		/* if the erase operation is completed, disable the SER Bit */
		FPEC->CR &= (~FPEC_CR_SER);
		FPEC->CR &= FPEC_SECTOR_MASK;
		
#endif
	}
	LEAVE_CRITICAL_SECTION();

	return ((ErrorStatus)res); 
}

//=============================================================================
ErrorStatus uni_flash_check_page_number(uint32_t page_number)
{
	if (page_number < FLASH_SECTORS_CNT)
	{
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

//=============================================================================
uint32_t uni_flash_get_page_address_start(uint32_t page_number)
{
	if (uni_flash_check_page_number(page_number) == ERROR)
	{
		return 0;
	}
	
	#if F1_CHECK || F37_CHECK
		return(page_number * FLASH_PAGE_SIZE + FLASH_START);
	#else
		if (page_number < 4)
		{
			return(page_number * FLASH_PAGE_SIZE_0_3 + FLASH_START);
		}
		else if (page_number == 4)
		{
			return(FLASH_SECTORS_CNT_0_3 * FLASH_PAGE_SIZE_0_3 + FLASH_START);
		}
		else
		{
			return((FLASH_SECTORS_CNT_0_3 * FLASH_PAGE_SIZE_0_3) + (FLASH_SECTORS_CNT_4 * FLASH_PAGE_SIZE_4) + ((page_number - (FLASH_PAGE_SIZE_0_3 + FLASH_SECTORS_CNT_4)) * FLASH_PAGE_SIZE_5_11) + FLASH_START);
		}
	#endif
}

//=============================================================================
uint32_t uni_flash_get_page_address_end(uint32_t page_number)
{
	if (uni_flash_check_page_number(page_number) == ERROR)
	{
		return 0;
	}
	
	#if F1_CHECK || F37_CHECK
		return(uni_flash_get_page_address_start(page_number) + FLASH_PAGE_SIZE - 1);
	#else
		if (page_number < 4)
		{
			return(uni_flash_get_page_address_start(page_number) + FLASH_PAGE_SIZE_0_3 - 1);
		}
		else if (page_number == 4)
		{
			return(uni_flash_get_page_address_start(page_number) + FLASH_PAGE_SIZE_4 - 1);
		}
		else
		{
			return(uni_flash_get_page_address_start(page_number) + FLASH_PAGE_SIZE_5_11 - 1);
		}
	#endif
}

//=============================================================================
void uni_flash_test(void)
{
	volatile uint32_t size = 255;
	volatile uint32_t page_start, page_end;
	volatile uint8_t save_to_flash[size];
	volatile uint8_t for_break_point;
	
	for (uint32_t i = 0; i < size; i++)
	{
		save_to_flash[i] = 0x0E;
	}
	
	page_start = uni_flash_get_page_address_start(127);
	page_end = uni_flash_get_page_address_end(127);
	
	for_break_point++;
	
	uni_flash_erase_page(page_start); //stm32f373: 22 ms
	
	for_break_point++;
	
	uni_flash_write_block((void*)page_start, (void*)save_to_flash, size); //stm32f373: 255 cnt = 7.05 ms, 1024 cnt = 27.95 ms
	
	for_break_point++;
	
	uni_flash_erase_page(page_start);
	
	for_break_point++;
}
