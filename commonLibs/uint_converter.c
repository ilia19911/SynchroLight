#include "include.h"
uint_converter 				converter;
#define CONVERT()
//
//================================================================================
void uint_converter_set_32bit(uint32_t data)
{
	converter.u32bit = data;
}
//================================================================================
void uint_converter_set_16bit(uint16_t data)
{
	converter.u32bit = (uint32_t)data;
}
//================================================================================
void uint_converter_set_8bit(uint8_t *data,	uint8_t  data_size)
{
	memcpy(converter.u8bit,data, data_size);
}
//================================================================================
void uint_converter_get_8bit(uint8_t *buffer,	uint8_t  data_size)
{
	memcpy(buffer, converter.u8bit,data_size);
}
//================================================================================
void uint_converter_get_8bit_invert(uint8_t *buffer,	uint8_t  data_size)
{
	uint8_t temp[data_size + 1];
	for(uint8_t i = 0; i < data_size; i ++)
	{
		temp[i] = converter.u8bit[data_size-i];
	}
	memcpy(buffer, converter.u8bit,data_size);
}
//================================================================================
uint32_t uint_converter_get_32bit()
{
	return converter.u32bit;
}
//================================================================================
