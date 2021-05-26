
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "include.h"

#define SERVO_ZERO_OFFSET			1010
#define SERVO_K_DIV					100	

#define STAND_PROTOCOL_BUF_SIZE 	1024 
#define PACKET_CRC					4 
#define TIMEOUT 					1000 //MS

#define ACCEL_MEAS_IN_ONE_PACK  	250


//rx buffer flag
#define BUFFER_NOT_BUSY 			0x00
#define BUFFER_RECEIVE_HEADER 		0x01
#define BUFFER_RECEIVE_DATA			0x03
#define BUFFER_BUSY 				0x04

//tx buffer flag
#define BUFFER_NOT_BUSY 			0x00
#define TRASMIT_MULTIPACK 			0x01

//func generic_number_of_pack
#define NOT_LAST_PACKET 			0x00
#define LAST_PACKET 				0x01

//external flag
#define ACCEL_MEASURE_COMPLETE 		0x01
#define ACCEL_MULTIPACK 			0x02
#define TASK_FLAG_EMPTY 			0x00



static		uint8_t _stand_packet_id[3] = {0xB0,0x3B,0x67};

 typedef union{
	uint32_t u32bit;
	uint8_t  u8bit[4];
} protocol_data_convert;


// packet types
enum{
null,
	Throttle_pack,
	Throttle_answer,
	
	Accel_measure_request,
	Accel_answer,
	Accel_measure_repeated_request,
	
	Request_averaged_measure,
	Averaged_measure,
	
	Adc_set_request,
	Adc_answer
};

#pragma pack(push, 1)
typedef struct{
		uint8_t  id[3];
		uint8_t	 type;
		uint8_t  len[2];
		uint8_t  alen[2];	
		uint8_t  packet_number[1];	
}_header;
#pragma pack(pop) 

typedef struct {
	_header *header;
	uint8_t *data;
	uint8_t *crc;
	protocol_data_convert converter;
}packet_structure;	

typedef struct{
		uint8_t			rx_buffer[256];
		uint8_t			tx_buffer[STAND_PROTOCOL_BUF_SIZE];

		uint64_t   		server_tumeout;
	
		int				temp_c;
		uint16_t 		len_rcv;
		uint8_t		 	packet_id[3];
		uint8_t 		header_siza;
	
		uint8_t			external_task_flag;
		uint8_t			flag;
		protocol_data_convert converter;
} stand_protocol;
	

void 		protocol_init(void);
void  		protocol_runner(void);
void 		protocol_set_task(stand_protocol *protocol, uint8_t _flag);

	
void		crc32hw_init		(void);
uint32_t	crc32hw				(const void *buf, int len, int clear);


#endif /* _PROTOCOL_H_ */

