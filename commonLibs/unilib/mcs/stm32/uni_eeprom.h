
#ifndef _UNI_EEPROM_H_
#define _UNI_EEPROM_H_

//=============================================================================
typedef struct
{
	struct
	{
		uint16_t page_number;
		uint8_t auto_page_erase_fl;
		uint8_t* data;
		uint32_t data_size;
	} in;
	
	struct
	{
		uint32_t page_address_start;
		uint32_t page_address_end;
		
		uint32_t data_address_for_write;
		uint32_t data_address_last_correct;
	} work;
	
	struct
	{
		uint32_t data_cnt;
		uint8_t data_find_fl;
		uint8_t data_load_fl;
		uint8_t data_save_fl;
		uint8_t data_save_error_fl;
		uint8_t data_size_overload_fl;
		uint8_t data_size_odd_fl;
		
		uint8_t page_empty_fl;
		uint8_t page_number_not_correct_fl;
		uint8_t page_overload_or_impossible_write_fl;
		
		uint8_t rw_possible_fl;		
	} out;
	
}uni_eeprom_t;

//=============================================================================
ErrorStatus uni_eeprom_init_and_get(uni_eeprom_t * unit);
ErrorStatus uni_eeprom_init(uni_eeprom_t * unit);
void uni_eeprom_reset_out(uni_eeprom_t * unit);
ErrorStatus uni_eeprom_check_in(uni_eeprom_t * unit);
ErrorStatus uni_eeprom_data_find(uni_eeprom_t * unit);
ErrorStatus uni_eeprom_data_load(uni_eeprom_t * unit);
FlagStatus uni_eeprom_check_page_empty(uni_eeprom_t * unit);
FlagStatus uni_eeprom_check_page_overload_or_impossible_write(uni_eeprom_t * unit);
ErrorStatus uni_eeprom_page_erase(uni_eeprom_t * unit);
ErrorStatus uni_eeprom_data_save(uni_eeprom_t * unit);
void uni_eeprom_test(void);


#endif
