#ifndef _BOOTCONF_H_
#define _BOOTCONF_H_

// Раскоментировать - если будет использоваться прошивка по UART
//#define BOOTLOADER_MODULE_UART		
		#define boot_uart								uart1						// Номер используемого UART
		#define boot_speed							115200						// Скорость порта

// Раскоментировать, если будет использоваться прошивка через радиоканал
//#define BOOTLOADER_MODULE_SX1276
//		#define	BOOTLOADER_RF_TRACT_CONTROL										// Используются переключение тракта - прием/передача
//		#define BOOTLOADER_RF_TRACT_PA_CONTROL									// Используются усилители на прием/передачу
		#define BOOTLOADER_RF_FREQUENCY				962000000					// Частота канала
		#define BOOTLOADER_SX_ADDRESS_BROADCAST		((uint16_t)0xffff)

// Область управления памятью
#define BOOTLOADER_APPLICATION_PAGE_NUMBER			8
#define BOOTLOADER_TOTAL_PAGES_NUMBER				128
#define BOOTLOADER_PAGE_SIZE						0x400
#define BOOTLOADER_APPLICATION_ADDRESS				0x08002000//(0x08000000 + (0x400 * BOOTLOADER_APPLICATION_PAGE_NUMBER))

#endif /* _BOOTCONF_H_ */
