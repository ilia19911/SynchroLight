#include "include.h"


//================================================================================
void set_mem_header(uint16_t scen_size, uint16_t scen_adress)
{
		my_memory.my_scen_header.start_adress 		= scen_adress;
		my_memory.my_scen_header.last_adress 			= scen_adress + scen_size;
		my_memory.my_scen_header.status						= SCEN_WRITED;
}
//================================================================================
//записывает в зону разметки хедер сцены  
void write_mem_header(uint16_t scen_index)
{
		my_memory.set_memory( (void*)(&my_memory.my_scen_header), sizeof(my_memory.my_scen_header), scen_index *sizeof(my_memory.my_scen_header));

}
//================================================================================
//возвращает индекс подходящего по размеру свободного места в памяти
static int16_t get_suitable_size(uint16_t Size)
{
		return 0;
}
//================================================================================
int8_t mem_save(uint8_t *pointer, uint16_t scen_size, uint16_t index)
{
		int16_t new_scen_adress = get_suitable_size(scen_size);
		if(new_scen_adress < 0) 
				return new_scen_adress; // если сцена занята
	
		// если все ок
		set_mem_header(scen_size, new_scen_adress);
		write_mem_header(index);
		my_memory.set_memory(pointer, scen_size, new_scen_adress);// бинго!
		return 0;
}
//================================================================================
//смотрим записана ли сцена, и возвращаем адрес
/// не известно как будет записываться хедер в структуру. Так как в структуре адреса 16 бит, а в памяти 8;
uint16_t fill_mem_header(uint16_t mem_number)
{
		my_memory.get_memory((void*)&my_memory.my_scen_header, sizeof(my_memory.my_scen_header), mem_number *sizeof(my_memory.my_scen_header));
		return 0;
} 
//================================================================================





