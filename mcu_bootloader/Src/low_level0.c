#include "include.h"

static void init_rcc (void);
static void init_gpio (void);

#ifdef BOOTLOADER_MODULE_SX1276
static void init_spi (void);
static void init_transceiver (void);
static void getSx1276Address (void);
#endif /* BOOTLOADER_MODULE_SX1276 */

/* ======================================================================	*/
/* Функция для внешнего вызова - инициирует всю нижнюю половину перефирии	*/
/* ======================================================================	*/
void initLowLevel (void)
{
	init_rcc ();
	init_gpio ();
	crc32hw_init ();

	#ifdef BOOTLOADER_MODULE_UART
	uart_init (&boot_uart, boot_speed);
	#endif

	#ifdef BOOTLOADER_MODULE_SX1276
	init_spi ();
	init_transceiver ();
	getSx1276Address ();
	#endif
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Включение предделителя на два входного тактового сигнала					*/
/* ======================================================================	*/
static void init_rcc (void)
{
	// Переключимся на тактование от HSI
	RCC_SYSCLKConfig (RCC_SYSCLKSource_HSI);
	while (RCC_GetSYSCLKSource() != 0x00);
	
	// Выставим новую конфигурацию PLL
	RCC_PLLCmd (DISABLE);
	RCC_PLLConfig (RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
	RCC_PLLCmd (ENABLE);
	while (RCC_GetFlagStatus (RCC_FLAG_PLLRDY) != SET);
	
	// Запускаем обратно тактование от PLL
	RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08);	
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Инициализация всех лапок													*/
/* ======================================================================	*/
static void init_gpio (void)
{
	// Включим тактование модуля альтернативных функций
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);

	// Выключаем все JTAG пины, оставляем только SWD
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

	// Инитим лапы
#ifdef BOOTLOADER_MODULE_UART
	pin_init (PIN_UART_TX);
	pin_init (PIN_UART_RX);
#endif /* BOOTLOADER_MODULE_UART */

#ifdef BOOTLOADER_MODULE_SX1276	
	pin_init (PIN_SPI_SCK);
	pin_init (PIN_SPI_MOSI);
	pin_init (PIN_SPI_MISO);
	pin_init (PIN_SPI_CS);
	pin_init (PIN_TR_RESET);
#ifdef BOOTLOADER_RF_TRACT_CONTROL
	pin_init (PIN_SWITCH_RX_EN);
	pin_init (PIN_SWITCH_TX_EN);
#endif /* BOOTLOADER_RF_TRACT_CONTROL */
#ifdef BOOTLOADER_RF_TRACT_PA_CONTROL
	pin_init (PIN_AMP_TX_EN);
	pin_init (PIN_AMP_RX_EN);
#endif /* BOOTLOADER_RF_TRACT_PA_CONTROL */
#endif /* BOOTLOADER_MODULE_SX1276 */
}
/* ======================================================================	*/

#ifdef BOOTLOADER_MODULE_SX1276	
/* ======================================================================	*/
/* Инициализация SPI для общения с трансиверами	и FLASH памятью				*/
/* SPI Mode 0																*/
/* Speed:	18 MHz															*/
/* First bit: MSB															*/
/* Datasize: 8 bit															*/
/* Mode: Master, full-duplex												*/
/* NSS: Software															*/
/* ======================================================================	*/
static void init_spi (void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_InitTypeDef sSPI;
	sSPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	sSPI.SPI_CPHA = SPI_CPHA_1Edge;
	sSPI.SPI_CPOL = SPI_CPOL_Low;
	sSPI.SPI_DataSize = SPI_DataSize_8b;
	sSPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	sSPI.SPI_FirstBit = SPI_FirstBit_MSB;
	sSPI.SPI_Mode = SPI_Mode_Master;
	sSPI.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init (SPI1, &sSPI);
	
	SPI_Cmd (SPI1, ENABLE);
}

static void init_transceiver (void)
{
	sx1276_LoRa_InitTypeDef sTrc;

	sTrc.CS_PORT = GPIOA;
	sTrc.CS_Pin = GPIO_Pin_4;
	sTrc.SPIx = SPI1;
	sTrc.RESET_PORT = GPIOB;
	sTrc.Reset_Pin = GPIO_Pin_0;

	sTrc.band = SX1276_LORA_BAND_HIGH;
	sTrc.bandWidth = SX1276_LORA_BANDWIDTH_500000;
	sTrc.crcPayload = SX1276_LORA_CRCPAYLOAD_DISABLE;
	sTrc.errorCodeRate = SX1276_LORA_ECR4_6;
	sTrc.frequency = BOOTLOADER_RF_FREQUENCY;
	sTrc.headerType = SX1276_LORA_HEADER_EXPLICIT;
	sTrc.maxPower = 7;
	sTrc.oscType = SX1276_LORA_OSC_TCXO;
	sTrc.outputPin = SX1276_LORA_OUT_RFO;
	sTrc.outputPower = 15;
	sTrc.preambleLength = 15;
	sTrc.spreadingFactor = 9;
	sTrc.oscillatorFrequency = RF_CLOCK_FREQUENCY;

	delay_ms (500);	// Ожидание POR трансивера
	sx1276_InitLoRaMode (&T, &sTrc);
	delay_ms (500);
	sx1276_LoRa_startRx (&T);
	
#ifdef BOOTLOADER_RF_TRACT_CONTROL	
	switchToRx ();
#endif /* BOOTLOADER_RF_TRACT_CONTROL */	
}

#ifdef BOOTLOADER_RF_TRACT_CONTROL
void switchToRx (void)
{
	#ifdef BOOTLOADER_RF_TRACT_PA_CONTROL
	pin_set (PIN_AMP_TX_EN);
	#endif
	pin_clr (PIN_SWITCH_TX_EN);
	pin_set (PIN_SWITCH_RX_EN);
	#ifdef BOOTLOADER_RF_TRACT_PA_CONTROL
	pin_clr (PIN_AMP_RX_EN);
	#endif
}

void switchToTx (void)
{
	#ifdef BOOTLOADER_RF_TRACT_PA_CONTROL
	pin_set (PIN_AMP_RX_EN);
	#endif
	pin_clr (PIN_SWITCH_RX_EN);
	pin_set (PIN_SWITCH_TX_EN);
	#ifdef BOOTLOADER_RF_TRACT_PA_CONTROL
	pin_clr (PIN_AMP_TX_EN);
	#endif
}
#endif /* BOOTLOADER_RF_TRACT_CONTROL */

static void getSx1276Address (void)
{
	uint32_t test;

	test = FLASH_GetUserOptionByte ();
	sx1276address = (test & 0x00ffff00) >> 8;
}
#endif /* #ifdef BOOTLOADER_MODULE_SX1276 */
