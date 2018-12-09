#include "include.h"

//=============================================================================
#define CNT_SIZE 4
#define CRC_SIZE 4
#define CNT_AND_CRC_SIZE (CNT_SIZE + CRC_SIZE)


//=============================================================================
ErrorStatus uni_eeprom_init_and_get(uni_eeprom_t * unit)
{
	if (uni_eeprom_init(unit) == ERROR)
	{
		return ERROR;
	}
	
	uni_eeprom_data_find(unit);
	uni_eeprom_data_load(unit);
	
	return SUCCESS;
}

//=============================================================================
ErrorStatus uni_eeprom_init(uni_eeprom_t * unit)
{
	uni_eeprom_reset_out(unit);
	
	if (uni_eeprom_check_in(unit) == ERROR)
	{
		return ERROR;
	}
	else
	{
		return SUCCESS;
	}
}

//=============================================================================
void uni_eeprom_reset_out(uni_eeprom_t * unit)
{
	unit->out.data_cnt = 0;
	unit->out.data_find_fl = 0;
	unit->out.data_load_fl = 0;
	unit->out.data_save_fl = 0;
	unit->out.data_save_error_fl = 0;
	unit->out.data_size_overload_fl = 0;
	unit->out.data_size_odd_fl = 0;
	unit->out.page_empty_fl = 0;
	unit->out.page_number_not_correct_fl = 0;
	unit->out.page_overload_or_impossible_write_fl = 0;
	unit->out.rw_possible_fl = 0;
}	

//=============================================================================
ErrorStatus uni_eeprom_check_in(uni_eeprom_t * unit)
{
	unit->out.page_number_not_correct_fl = 0;
	unit->out.data_size_overload_fl = 0;
	unit->out.data_size_odd_fl = 0;
	
	if (uni_flash_check_page_number(unit->in.page_number) == ERROR)
	{
		unit->out.page_number_not_correct_fl = 1;
	}
	
	unit->work.page_address_start = uni_flash_get_page_address_start(unit->in.page_number);
	unit->work.page_address_end = uni_flash_get_page_address_end(unit->in.page_number);
	
	if ((unit->in.data_size + CNT_AND_CRC_SIZE) > (unit->work.page_address_end - unit->work.page_address_start))
	{
		unit->out.data_size_overload_fl = 1;
	}
	
	if (unit->in.data_size % 2)
	{
		unit->out.data_size_odd_fl = 1;
	}
	
	if (unit->out.page_number_not_correct_fl || unit->out.data_size_overload_fl || unit->out.data_size_odd_fl)
	{
		unit->out.rw_possible_fl = 0;
		return ERROR;
	}
	else
	{
		unit->out.rw_possible_fl = 1;
		return SUCCESS;
	}
}

//=============================================================================
ErrorStatus uni_eeprom_data_find(uni_eeprom_t * unit)
{	
	if (!unit->out.rw_possible_fl)
	{
		return ERROR;
	} 
	
	uni_eeprom_check_page_empty(unit);
	
	unit->out.data_find_fl = 0;
	
	uint32_t crc, crc_from_flash;
	
	if (!unit->out.page_empty_fl)
	{
		for(uint32_t data_adress_start = unit->work.page_address_start; data_adress_start < (unit->work.page_address_end - unit->in.data_size - CNT_AND_CRC_SIZE); data_adress_start += (unit->in.data_size + CNT_AND_CRC_SIZE))
		{		
			crc = crc32hw((void*)data_adress_start, unit->in.data_size, 1);
			
			crc_from_flash = *(uint32_t *)(data_adress_start + unit->in.data_size + CNT_SIZE);
			
			if(crc == crc_from_flash)
			{				
				unit->work.data_address_last_correct = data_adress_start;
				unit->out.data_cnt = *(uint32_t *)(data_adress_start + unit->in.data_size);
				unit->out.data_find_fl = 1;
			}
			else
			{
				break;
			}
		}
		
		if (unit->out.data_find_fl)
		{
			unit->work.data_address_for_write = unit->work.data_address_last_correct + unit->in.data_size + CNT_AND_CRC_SIZE;
		}
		else
		{
			if (unit->in.auto_page_erase_fl)
			{
				uni_eeprom_page_erase(unit);
			}
			else
			{
				unit->work.data_address_for_write = unit->work.page_address_start;
			}
		}
	}
	else
	{
		unit->work.data_address_for_write = unit->work.page_address_start;
	}
	
	uni_eeprom_check_page_overload_or_impossible_write(unit);
	
	return SUCCESS;
}

//=============================================================================
ErrorStatus uni_eeprom_data_load(uni_eeprom_t * unit)
{
	if (!unit->out.rw_possible_fl || !unit->out.data_find_fl)
	{
		unit->out.data_load_fl = 0;
		return ERROR;
	}
	
	memcpy(unit->in.data, (void*)unit->work.data_address_last_correct, unit->in.data_size);
	
	unit->out.data_load_fl = 1;
	
	return SUCCESS;
}

//=============================================================================
FlagStatus uni_eeprom_check_page_empty(uni_eeprom_t * unit)
{
	if (!unit->out.rw_possible_fl)
	{
		return RESET;
	} 
	
	for (uint32_t i = unit->work.page_address_start; i < (unit->work.page_address_end + 1); i += 4)
	{
		if (*(uint32_t *)i != 0xFFFFFFFF)
		{
			unit->out.page_empty_fl = 0;
			return RESET;
		}
	}
	
	unit->out.page_empty_fl = 1;
	return SET;
}

//=============================================================================
FlagStatus uni_eeprom_check_page_overload_or_impossible_write(uni_eeprom_t * unit)
{
	if (!unit->out.rw_possible_fl)
	{
		return RESET;
	}
	
	if ((unit->work.data_address_for_write + unit->in.data_size + CNT_AND_CRC_SIZE) > unit->work.page_address_end)
	{
		unit->out.page_overload_or_impossible_write_fl = 1;
		return SET;
	}
	else
	{
		for (uint32_t i = unit->work.data_address_for_write; i < (unit->work.data_address_for_write + unit->in.data_size + CNT_AND_CRC_SIZE); i += 2)
		{
			if (*(uint16_t *)i != 0xFFFF)
			{
				unit->out.page_overload_or_impossible_write_fl = 1;
				return SET;
			}
		}
		
		unit->out.page_overload_or_impossible_write_fl = 0;
		return RESET;
	}
}

//=============================================================================
ErrorStatus uni_eeprom_page_erase(uni_eeprom_t * unit)
{
	if (!unit->out.rw_possible_fl)
	{
		return ERROR;
	} 
	
	uni_flash_erase_page(unit->work.page_address_start);
	
	unit->work.data_address_for_write = unit->work.page_address_start;
	unit->out.page_empty_fl = 1;
	unit->out.data_find_fl = 0;
	
	uni_eeprom_check_page_overload_or_impossible_write(unit);
	
	return SUCCESS;
}

//=============================================================================
ErrorStatus uni_eeprom_data_save(uni_eeprom_t * unit)
{
	if (!unit->out.rw_possible_fl)
	{
		return ERROR;
	} 
	
	unit->out.data_save_fl = 0;
	unit->out.data_save_error_fl = 0;
	
	uni_eeprom_check_page_overload_or_impossible_write(unit);
	
	if (unit->out.page_overload_or_impossible_write_fl)
	{
		if (unit->in.auto_page_erase_fl)
		{
			uni_eeprom_page_erase(unit);			
		}
		else
		{
			unit->out.data_save_error_fl = 1;
			return ERROR;
		}
	}
	
	uint8_t i, data_write_fl = 0, crc_write_fl = 0, cnt_write_fl = 0;
	uint32_t data_address_for_write_save = unit->work.data_address_for_write;
	uint32_t crc_to_flash = crc32hw(unit->in.data, unit->in.data_size, 1);
		
	for (i = 0; i < 3; i++)
	{
		if (uni_flash_write_block((void *)unit->work.data_address_for_write, unit->in.data, unit->in.data_size) == SUCCESS)
		{
			unit->work.data_address_for_write += unit->in.data_size;
			data_write_fl = 1;
			break;
		}		
	}

	if (data_write_fl)
	{
		unit->out.data_cnt++;
		
		for (i = 0; i < 3; i++)
		{
			if (uni_flash_write_block((void *)unit->work.data_address_for_write, (void *)&unit->out.data_cnt, CNT_SIZE) == SUCCESS)
			{
				unit->work.data_address_for_write += 4;
				cnt_write_fl = 1;
				break;
			}		
		}
		
		if (!cnt_write_fl)
		{
			unit->out.data_cnt--;
		}
	}
	
	if (data_write_fl && cnt_write_fl)
	{
		for (i = 0; i < 3; i++)
		{
			if (uni_flash_write_block((void *)unit->work.data_address_for_write, (void *)&crc_to_flash, CRC_SIZE) == SUCCESS)
			{
				unit->work.data_address_for_write += 4;
				crc_write_fl = 1;
				break;
			}		
		}
	}
			
	uni_eeprom_check_page_overload_or_impossible_write(unit);

	if (data_write_fl && cnt_write_fl && crc_write_fl)
	{
		unit->out.data_find_fl = 1;
		unit->work.data_address_last_correct = data_address_for_write_save;
		
		unit->out.data_save_fl = 1;
		return SUCCESS;
	}
	else
	{
		unit->out.data_save_error_fl = 1;
		return ERROR;
	}
}

//=============================================================================
void uni_eeprom_test(void)
{
	uni_eeprom_t uni_eeprom;
	volatile uint32_t cnt_write = 1500;
	volatile uint8_t size = 6;
	volatile ErrorStatus status;
	volatile uint8_t for_break_point;	
	volatile uint8_t tst_data[size];
	
	uni_eeprom.in.data = (uint8_t*)&tst_data;
	uni_eeprom.in.data_size = size;
	uni_eeprom.in.auto_page_erase_fl = 1;
	uni_eeprom.in.page_number = 127;
		
	for_break_point++;
	
	status = uni_eeprom_init_and_get(&uni_eeprom);
	
	for_break_point++;
	
	//status = uni_eeprom_page_erase(&uni_eeprom);
	
	for_break_point++;
	
	for (uint32_t j = 0; j < cnt_write; j++)
	{
		for_break_point++;
		
		for (uint8_t i = 0; i < size; i++)
		{
			tst_data[i]++;
		}
		
		status = uni_eeprom_data_save(&uni_eeprom);
	}
	
	tst_data[0] = 0x01;
	
	status = uni_eeprom_data_find(&uni_eeprom);
	
	for_break_point++;
		
	status = uni_eeprom_data_load(&uni_eeprom);
	
	for_break_point++;
	
	uint8_t bad_data[2] = {0xEF, 0xEA};
	uni_flash_write_block((void*)(uni_eeprom.work.data_address_for_write+2), (void*)&bad_data, 2);
	
	for_break_point++;
	
	status = uni_eeprom_data_save(&uni_eeprom);
	
	for_break_point++;
	
	status = uni_eeprom_data_load(&uni_eeprom);
	
	for_break_point++;
	
	//status = uni_eeprom_page_erase(&uni_eeprom);
	
	for_break_point++;
	
	status = uni_eeprom_data_save(&uni_eeprom);
	
	for_break_point++;
}
