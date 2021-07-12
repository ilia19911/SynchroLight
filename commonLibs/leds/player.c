#include "include.h"

//#include "rf_parser.h"
#include "ws2812b.h"

#define START_SLAVE
#define MEMORY
#define TEST_MODULE

#ifdef TEST_MODULE
uint8_t temp_mem[25 * 21] = {
	//	[ ?????? ???.....[??????? ????.........................................................................................]
	//1//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 45, 1, 0, 0, 0, 0, 10, 1, 0, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0,		   // 1 SCEN
																				   //2//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 200, 0, 45, 1, 0, 0, 10, 1, 0, 255, 0, 100, 0, 0, 0, 0, 0, 0, 0,		   // 2-3 scen
																				   //3//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 200, 0, 45, 1, 0, 0, 10, 1, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0,		   // 4-5 scen
																				   //4//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 200, 0, 45, 1, 1, 1, 30, 2, 0, 3, 255, 0, 50, 0, 0, 255, 255, 0, 50,	   // 6-7 scen
																				   //5//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 200, 0, 45, 1, 1, 1, 30, 2, 0, 3, 255, 0, 50, 255, 255, 255, 0, 0, 255,  // 8-9 scen
																				   //6//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 200, 0, 45, 1, 1, 1, 30, 2, 0, 3, 0, 0, 255, 255, 0, 50, 255, 255, 0255, // 10-11 scen
																				   //7//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 0, 50, 255, 255, 255, 0, 0, 255,	   // 12 scen
																				   //8//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 255, 255, 0, 0, 255, 255, 0, 50,	   // 13 scen
																				   //9//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 0, 0, 255, 255, 0, 50, 255, 255, 255,	   // 14 scen
																				   //10//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 0, 50, 255, 255, 255, 0, 0, 255,	   // 12 scen
																				   //11//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 255, 255, 0, 0, 255, 255, 0, 50,	   // 13
																				   //12//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 0, 0, 255, 255, 0, 50, 255, 255, 255,	   // 14
																				   //13//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 0, 50, 255, 255, 255, 0, 0, 255,	   // 12 ??
																				   //14//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 255, 255, 0, 0, 255, 255, 0, 50,	   //13
																				   //15//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 0, 0, 255, 255, 0, 50, 255, 255, 255,	   //14
																				   //16//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 0, 50, 255, 255, 255, 0, 0, 255,	   // 12 ??
																				   //17//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 255, 255, 255, 0, 0, 255, 255, 0, 50,	   //13
																				   //18//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 50, 0, 0, 0, 1, 1, 30, 2, 0, 3, 0, 0, 255, 255, 0, 50, 255, 255, 255,	   //14
																				   //19//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 45, 1, 0, 0, 1, 1, 30, 2, 0, 3, 0, 0, 255, 255, 0, 50, 255, 255, 255,	   //14
																				   //20//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb
	1, 0, 200, 0, 0, 0, 0, 0, 10, 1, 0, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0		   ///  ???? ????, ???2??

};
#endif

/*
header memory structure
number




once mem
[0] - number
[1] - number

[3] - scens
[4] - scens

[5] - adress 1 scen
[5] - adress 1 scen

[n+5] - adress n scen


	n scen structure
	[0] - macro comand
	[1] - macro comand
	[2]   - data
	[2+n] - data
	
		data structur
		if macro = 1 (color transition)
		[0] - new red color
		[1] - new green color
		[2] - new blue color
		[3] - x offset (8 bit - revers (right to left))
		[4] - y offset (8 bit - revers (right to left))
		[5] - commun speed low bit
		[6] - commun speed hight bit


	

*/
#if !defined MASTER && !defined MASTER_V2

//================================================================================
////
//static uint16_t get_frame_adress(uint16_t mem_number, uint16_t frame_number)
//{
//		if(frame_number	> PLAYBACK_HEADER.number_of_frame) return 0; //
//		my_memory.get_memory(my_coverter.separat_data, sizeof(uint16_t), my_memory.my_scen_header.start_adress + sizeof(PLAYBACK_HEADER) + frame_number*sizeof(uint16_t) );//
//
//		return my_coverter.union_state; //
//}
//================================================================================
//static uint16_t get_frame_size(uint16_t mem_number, uint16_t frame_number)
//{
//		if(frame_number	< PLAYBACK_HEADER.number_of_frame) //
//			 return (get_frame_adress(mem_number, frame_number + 1) - 1) - get_frame_adress(mem_number, frame_number); //
//	if(frame_number	== PLAYBACK_HEADER.number_of_frame) //
//			return my_memory.my_scen_header.last_adress - get_frame_adress(mem_number, frame_number); //
//}



static void set_led_state()
{
	leds this_led;
	for (uint16_t x = 0; x < X_MATRIX_SIZE; x++)
	{
		for (uint16_t y = 0; y < Y_MATRIX_SIZE; y++)
		{
			handler_definition(FRAME, x, y, &this_led, my_player.crosfade);
			led_map_set_pixel(x, y, this_led);
		}
	}
}

#endif


static void TEST_get_frame(uint16_t frame_number)
{
	memcpy((void *)(&FRAME), temp_mem + (frame_number * 21), 21); //
}
static void fill_frame(uint16_t mem_number, uint16_t frame_number)
{
#ifndef TEST_MODULE
	uint16_t frame_adress = get_frame_adress(mem_number, frame_number);
	uint16_t frame_size = get_frame_size(mem_number, frame_number);
	my_memory.get_memory((void *)&FRAME_HEADER, frame_size, frame_adress);
#endif
#ifdef TEST_MODULE
	//memcpy((void*)(&FRAME + (frame_number*20)),temp_mem,20);//
	TEST_get_frame(frame_number);
#endif
}
static void auto_start_next_frame()
{
	my_player.frame++; //
#ifdef TEST_MODULE
	if (my_player.frame == FRAME_IN_TEST_SCEN )
		my_player.frame = 0;
#endif
	player_play_mem(my_player.mem, my_player.frame, 1);
#if defined MASTER || defined MASTER_V2
	rf_sendStartCmd(RF_ADDRESS_BROADCAST, my_player.mem, my_player.frame, 0, my_player.crosfade); // �������� ������� ������� �����
#endif
}
static void fill_playback_header()
{
	my_memory.get_memory((void *)&PLAYBACK_HEADER, sizeof(PLAYBACK_HEADER), my_memory.my_scen_header.start_adress);
}
static uint8_t check_saturation(uint32_t val)
{
	uint8_t result = val;
	if(val>0xFF)
	{
		result = 0xFF;
	}	
	return result;
}
static uint8_t holdtime_calc()
{
	uint8_t result = 0;
	if(FRAME.holdtime!=0)
	{
		uint32_t new_holdfade = (((systick_timer.tics - my_player.frame_time_begine) - (FRAME.crostime * 10)) * 255) / (FRAME.holdtime * 10);
		if (my_player.hold != check_saturation(new_holdfade) || my_player.crosfade==0xFF)
		{
			my_player.hold = check_saturation(new_holdfade);
			if (my_player.hold == 0xFF)
			{
				auto_start_next_frame(); 
			}
			result = 1;
		}
	}
	else
	{
		auto_start_next_frame(); 
		result = 1;
	}
	return result;
}
static uint8_t crosfade_calc()
{
	uint8_t result = 0;
	uint32_t new_crossfade = ((systick_timer.tics - my_player.frame_time_begine) * 255) /(FRAME.crostime * 10);
	if (my_player.crosfade != check_saturation(new_crossfade))
	{
		my_player.crosfade = check_saturation(new_crossfade);
		result = 1;
	}
	else if( my_player.crosfade==0xFF)
	{
		result = holdtime_calc();
	}
	return result;
}
void payer_handler(void)
{
	if (my_player.state == PLAYBACK_ENABLED)
	{
		if (crosfade_calc() )
		{
			#if !defined MASTER && !defined MASTER_V2
			set_led_state();
			#endif
		}
	}
}
void player_stop_mem()
{
	my_player.state = PLAYBACK_DISABLED; //
#if !defined MASTER && !defined MASTER_V2
	for (uint8_t i = 0; i < LED_NUMBER; i++)
	{
		my_map.my_leds[i].red = 0;
		my_map.my_leds[i].green = 0;
		my_map.my_leds[i].blue = 0;
	}
#endif
#if defined MASTER || defined MASTER_V2
	//#ifdef M/StopCmd (RF_ADDRESS_BROADCAST);
	//#endif
	rf_sendStopCmd(RF_ADDRESS_BROADCAST);
#endif
}
void player_synchro(uint16_t mem_number, uint8_t mem_frame, int8_t rate, uint8_t crossfade)
{
	player_play_mem(mem_number, mem_frame, 1);
}
void player_play_mem(uint16_t mem_number, uint8_t mem_frame, uint8_t rate)
{
#ifndef TEST_MODULE
	fill_mem_header(mem_number); //
	fill_playback_header();		 //
#endif

	if(mem_frame!=0)
	{
		fill_frame(mem_number, mem_frame-1);
	}
	else
	{
		fill_frame(mem_number, FRAME_IN_TEST_SCEN-1);
	}
	PREVIOUS_FRAME = FRAME;
	fill_frame(mem_number, mem_frame);
	my_player.mem = mem_number;
	my_player.frame = mem_frame;
	my_player.state = PLAYBACK_ENABLED;				  //
	my_player.frame_time_begine = systick_timer.tics; //
	my_player.crosfade = 0;
	my_player.hold = 0;
}