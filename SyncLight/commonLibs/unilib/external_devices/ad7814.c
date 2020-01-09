#include "include.h"

static uint8_t spiXmit (SPI_TypeDef *SPIx, uint8_t byte);
static uint16_t readValue (ad7814_t *sense);
	
void ad7814Task (ad7814_t *sense)
{
	int16_t reg;
	
	// Считаем слово из датчика
	reg = readValue (sense);
	
	// В этих 16 битах нас интересует следующее:
	// Самый старший бит - всегда ноль.
	// Последние 5 бит - всегда единицы.
	// В результате можно просто сдвинуть все слово вправо на 5 бит и получим предварительный результат
	reg >>= RESULT_DATA_OFFSET;
	
	// Теперь надо восстановить знак, поэтому если девятый бит равен 1, то необходимо выставить 1 и во всех остальных битах до 15-го
	if (reg & 0x0200) reg |= 0xfc00;
	
	// Ну и получить значение в градусах цельсия
	sense->temperature = (float)reg * KScale;
		
	__nop ();
}

static uint8_t spiXmit (SPI_TypeDef *SPIx, uint8_t byte)
{
	SPIx->DR = byte;
	while (!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}

static uint16_t readValue (ad7814_t *sense)
{
	uint16_t temp;
	sense->CS_PORT->ODR &=~ sense->CS_PIN;
	temp = spiXmit (sense->SPIx, 0x00) << 8;
	temp |= spiXmit (sense->SPIx, 0x00);
	sense->CS_PORT->ODR |= sense->CS_PIN;
	return temp;
}
