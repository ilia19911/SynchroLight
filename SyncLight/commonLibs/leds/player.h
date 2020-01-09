#include "include.h"
#ifndef _PLAYER_H
#define _PLAYER_H

//state �������
#define PLAYBACK_ENABLED	  1
#define PLAYBACK_DISABLED 	0


#define SYNCHRO_VALUE 20
#define DOPUSK 10

#define FRAME_IN_TEST_SCEN	20



#define HEADER_MEM_SIZE		100
#define HEADER_FRAME_SIZE	100

// ��������� ��������� ���������������
#pragma pack(push, 1)
typedef struct 
{
		uint16_t 	instruction;						 				 //���������� �� ��������������� ������
		uint16_t	crostime;												 // ����� ��������������� � 0.1 ��
		uint16_t	holdtime;												 // ����� ��������������� � 0.1 ��
		uint8_t 	buffer[HEADER_FRAME_SIZE]; 			 // ����� ��� ������ ����
																							 // ��������� ��������
}frame;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct 
{	
		uint16_t		mem_number;											//����� ����� � ������
		uint16_t		number_of_frame; 								//���������� ���� � ����
}playback;
#pragma pack(pop)

typedef struct
{
	playback 	playback_header;
	frame 		frame_header;
	frame 		previous_frame;
	uint8_t 	state;
	
	uint8_t		mem; 							 									// ����� ���� ���������������
	uint8_t		frame; 															// ����� ������ ���� ���������������
	uint8_t 	rate; 						 									// ������������� �������� ���������������
	uint8_t 	crosfade;														//���������� ��������. �� 0 �� 1;
	uint8_t 	hold;																//���������� ��������. �� 0 �� 1;
	uint32_t	frame_time_begine;									// ����� ��������� �����
	
}player;

void player_start_mem(uint16_t mem_number, uint8_t mem_frame,	int8_t rate);
void player_stop_mem(void);
void payer_handler(void);
void player_play_mem(uint16_t mem_number, uint8_t mem_frame,	uint8_t rate);
void player_start_mem(uint16_t mem_number, uint8_t mem_frame,	int8_t rate);
void player_synchro(uint16_t mem_number, uint8_t mem_frame,	int8_t rate, uint8_t crossfade);

#endif

