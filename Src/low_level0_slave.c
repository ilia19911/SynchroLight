#include "include.h"

static void init_rcc (void);
static void init_gpio (void);
static void init_spi (void);
//static void init_w2812_B10_struct(void);
//static void init_w2812_B11_struct(void);
//static void init_w2812_TIM1_struct(void);

/* ======================================================================	*/
/* Функция для внешнего вызова - инициирует всю нижнюю половину перефирии */
/* ======================================================================	*/
void initLowLevel (void)
{
	init_rcc ();
	init_gpio ();
	init_spi ();
	initTransceiver ();
	//uart_init (&uart_debug, 115200);

	//dprintf ("Synchro light v%d.%d%s starting...\r\n", VERSION_MAJOR, VERSION_MINOR, VERSION_LITERAL);

}
/* ======================================================================	*/

/* ======================================================================	*/
/* Включение предделителя на два входного тактового сигнала								*/
/* ======================================================================	*/
static void init_rcc (void)
{
	RCC->CFGR &=~ RCC_CFGR_SW;								// Переключаемся на тактование от встроенного генератора
	while (RCC->CFGR & RCC_CFGR_SWS);					// Ждем пока переключимся
	
	RCC_PLLCmd (DISABLE);
	RCC_PLLConfig (RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
	RCC_PLLCmd (ENABLE);

	while (!(RCC->CR & RCC_CR_PLLRDY));				// Ждем захвата ФАПЧ
	RCC->CFGR |= RCC_CFGR_SW_PLL;							// Переключаем тактование на PLL	
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));	// Ждем пока переключимся
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);				// Включаием AFIO
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);				// Включаем второй таймер
	//RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);				// Включаем второй таймер
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);				// Включаем первый ДМА
	

	//RCC->CFGR &=~ RCC_CFGR_SW;								// Переключаемся на тактование от встроенного генератора
	//while (RCC->CFGR & RCC_CFGR_SWS);					// Ждем пока переключимся
	//
	//// Умножаем теперь на на 9, а на 8 PLL. Нам нужна частота в 64 МГц, а на MCO пойдет 32
	//RCC_PLLCmd (DISABLE);
	//RCC_PLLConfig (RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
	//RCC_PLLCmd (ENABLE);
  //
	//while (!(RCC->CR & RCC_CR_PLLRDY));				// Ждем захвата ФАПЧ
	//RCC->CFGR |= RCC_CFGR_SW_PLL;							// Переключаем тактование на PLL	
	//while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));	// Ждем пока переключимся
	//
	//// Включим MCO как PLL/2
	//RCC_MCOConfig (RCC_MCO_PLLCLK_Div2);
	//
	//RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);				// Включаием AFIO
	//RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);				// Включаем второй таймер
	//RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);				// Включаем второй таймер
	//RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);				// Включаем первый ДМА
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Инициализация всех лапок																								*/
/* ======================================================================	*/
static void init_gpio (void)
{
	// Выключаем все JTAG пины, оставляем только SWD
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;
	
	// Ремап пинов второго таймера
	//GPIO_PinRemapConfig (GPIO_FullRemap_TIM2, ENABLE);
	
	// Инитим лапы
	pin_init (PIN_UART1_TX);
	pin_init (PIN_UART1_RX);
	pin_init (PIN_UART2_TX);
	pin_init (PIN_UART2_RX);
	pin_init (PIN_UART3_TX);
	pin_init (PIN_UART3_RX);
	pin_init (PIN_TRC_RESET);
	pin_init (PIN_SPI_TRC_SCK);
	pin_init (PIN_SPI_TRC_MOSI);
	pin_init (PIN_SPI_TRC_MISO);
	pin_init (PIN_SPI_TRC_CS);
	pin_init (PIN_SPI_FLASH_SCK);
	pin_init (PIN_SPI_FLASH_MOSI);
	pin_init (PIN_SPI_FLASH_MISO);
	pin_init (PIN_SPI_FLASH_SD_CS);
	pin_init (PIN_SPI_FLASH_AT_CS);

}

/* Инициализация SPI для общения с трансиверами
 SPI Mode 0
 Speed:	18 MHz
 First bit: MSB
 Datasize: 8 bit
 Mode: Master, full-duplex
 NSS: Software							
*/ 
static void init_spi (void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE);
	
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

void initTransceiver (void)
{
	sx1276_LoRa_InitTypeDef sTrc;
	
	sTrc.CS_PORT = GPIOA;
	sTrc.CS_Pin = GPIO_Pin_4;
	sTrc.SPIx = SPI1;
	
	sTrc.band = SX1276_LORA_BAND_HIGH;
	sTrc.bandWidth = SX1276_LORA_BANDWIDTH_500000;
	sTrc.crcPayload = SX1276_LORA_CRCPAYLOAD_DISABLE;
	sTrc.errorCodeRate = SX1276_LORA_ECR4_6;
	sTrc.frequency = SERVICE_FREQUENCY;
	sTrc.headerType = SX1276_LORA_HEADER_EXPLICIT;
	sTrc.maxPower = 7;
	sTrc.oscType = SX1276_LORA_OSC_TCXO;
	sTrc.outputPin = SX1276_LORA_OUT_RFO;
	sTrc.outputPower = 15;
	sTrc.preambleLength = 10;
	sTrc.spreadingFactor = 9;
	sTrc.oscillatorFrequency = RF_CLOCK_FREQUENCY;

	// Резет трансивера
	pin_clr (PIN_TRC_RESET);
	delay_ms (100);
	pin_set (PIN_TRC_RESET);
	
	// Подождем секунду, чтобы у трансиверов прошел полностью POR
	delay_ms (1000);

	sx1276_InitLoRaMode (&transc, &sTrc);
	
	// Подождем чутка еще
	delay_ms (500);
	
	software_timer_start (&transc_update, 10);
	
	// Разрешаем прием первым трансивером
	sx1276_LoRa_startRx (&transc);	
}

void transceiverTask (void)
{
	if (software_timer (&transc_update))
		sx1276_LoRa_updateState (&transc);

	//if (!transc.Control.busy) pin_clr (PIN_DIGIT_EN_4);
	
	if (transc.receiver.readyRead)
	{
		// Тут колбэк на обработку принятых данных
		transc.receiver.readyRead = 0;
	}
}








