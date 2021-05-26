#ifndef _AD7814_H_
#define _AD7814_H_

#define RESULT_DATA_OFFSET							5
#define KScale													((float)0.25)

typedef struct 
{
	GPIO_TypeDef *CS_PORT;
	uint32_t CS_PIN;
	SPI_TypeDef *SPIx;
	float temperature;
} ad7814_t;

void ad7814Task (ad7814_t *sense);

#endif /* _AD7814_H_ */
