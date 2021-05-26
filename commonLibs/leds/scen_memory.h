#include "include.h"

#define SCEN_MEMORY_H
#ifdef SCEN_MEMORY_H

#define MAX_NUMBER_OF_SCEN 100



//ошибки
#define  SCEN_ALREADY_TAKEN  -1   //сцена занята.


//флаги записвнных сцен
#define SCEN_WRITED 					1

typedef	union {
			uint16_t union_state;
			uint8_t separat_data[2];
}coverter;


#pragma pack(push, 1)
typedef struct 
{
		uint16_t start_adress;
	  uint16_t last_adress;
	  uint8_t  status;
}scen_header;
#pragma pack(pop)


typedef struct 
{
		scen_header my_scen_header;
		uint8_t (*get_memory)(uint8_t *array, uint16_t size, uint16_t adress);
		uint8_t (*set_memory)(uint8_t *array, uint16_t size, uint16_t adress);
		
}main_memory;

uint16_t fill_mem_header(uint16_t mem_number);


#endif

