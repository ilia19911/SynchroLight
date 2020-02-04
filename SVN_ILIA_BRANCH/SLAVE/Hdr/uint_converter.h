#include "include.h"
/*

The module is designed to quickly convert the number of uint32 or uint16 to uint8. You can also get an inverted byte sequence, (starts with HBt)
*/
 typedef union{
	uint32_t u32bit;
	uint8_t  u8bit[4];
} uint_converter;

void		uint_converter_set_32bit(uint32_t data);
void 		uint_converter_set_16bit(uint16_t data);
void		uint_converter_set_8bit(uint8_t *data,	uint8_t  data_size);
void 		uint_converter_get_8bit(uint8_t *buffer, uint8_t  data_size);
uint32_t	uint_converter_get_32bit(void);
