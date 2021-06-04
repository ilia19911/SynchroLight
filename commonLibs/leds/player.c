#include "include.h"
//#include "rf_parser.h"
#include "ws2812b.h"

#define START_SLAVE

#define MEMORY 		

#define TEST_MODULE

	static uint8_t synchro_flag = 0;

#ifdef TEST_MODULE
uint8_t temp_mem[25*21]={				//	[ ?????? ???.....[??????? ????.........................................................................................]
//1//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb		
		1,0,			45,1,				0,0,				0,0,						10,								1, 0,							255,255,255,            		0,0,0,0,0,0,0, 									// 1 SCEN
//2//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb				
		1,0,			200,0,				45,1,				0,0,						10,								1, 0,							255,0,100,           	 			0,0,0,0,0,0,0,									// 2-3 scen
//3//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb				
		1,0,			200,0,				45,1,				0,0,						10,								1, 0,							0,0,255,            				0,0,0,0,0,0,0,									// 4-5 scen
//4//	instr		time fade			time hold			route					fade						 		type_of processor					 rgb		
		1,0,			200,0,				45,1,				1,1,						30,								2, 0,							3,				255,0,50,					0,0,255,					255,0,50,   	// 6-7 scen
//5//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb			                           
		1,0,			200,0,				45,1,				1,1,						30,								2, 0,							3,				255,0,50,					255,255,255,			0,0,255, 			// 8-9 scen
//6//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb							                                        
		1,0,			200,0,				45,1,				1,1,						30,								2, 0,							3,				0,0,255,					255,0,50,					255,255,0255,	// 10-11 scen
//7//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb			                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,0,50,					255,255,255,			0,0,255,			// 12 scen
//8//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb							                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,255,255,			0,0,255,					255,0,50,			// 13 scen
//9//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb								                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				0,0,255,					255,0,50,					255,255,255,	// 14 scen
//10//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb						                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,0,50,					255,255,255,			0,0,255,			// 12 scen
//11//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb							                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,255,255,			0,0,255,					255,0,50,			// 13
//12//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb						                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				0,0,255,					255,0,50,					255,255,255,	// 14
//13//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb						                         							
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,0,50,					255,255,255,			0,0,255,			// 12 ??
//14//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb					                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,255,255,			0,0,255,					255,0,50,			//13
//15//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb						                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				0,0,255,					255,0,50,					255,255,255,		//14
//16//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb					                         							
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,0,50,					255,255,255,			0,0,255,				// 12 ??
//17//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb						                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				255,255,255,			0,0,255,					255,0,50,			//13
//18//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb							                                        
		1,0,			50,0,				0,0,				1,1,						30,								2, 0,							3,				0,0,255,					255,0,50,					255,255,255,		//14
//19//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb							                                        
		1,0,			45,1,				0,0,				1,1,						30,								2, 0,							3,				0,0,255,					255,0,50,					255,255,255,		//14
//20//	instr		time fade		time hold			route					fade						 		type_of processor					 rgb																			 							
		1,0,			200,0,			0,0,			0,0,						10,									1, 0,							255,255,255,            		0,0,0,0,0,0,0 		///  ???? ????, ???2??

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
#ifndef MASTER

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


//

//================================================================================
void set_led_state()
{
	leds this_led;
	for(uint16_t x = 0; x < X_MATRIX_SIZE; x++)
	{
		for(uint16_t y = 0; y < Y_MATRIX_SIZE; y++)
				{
						if (software_timer (&transc_update)) sx1276_LoRa_updateState (&transc);
						if (transc.receiver.readyRead)
						{
								rf_parse (transc.receiver.rxBuffer, transc.receiver.bytesReceived);
								transc.receiver.readyRead = 0;
								return;
						}
					 handler_definition(FRAME, x, y, &this_led, my_player.crosfade);
					 led_map_set_pixel(x, y, this_led);
				}
	}
}

//================================================================================
void player_comand_handler()
{
	
}

 //================================================================================

#endif

//================================================================================
void TEST_get_frame(uint16_t frame_number)
{
	memcpy((void*)(&FRAME ),temp_mem + (frame_number*21),21);//
}
//================================================================================
void fill_frame(uint16_t mem_number, uint16_t frame_number)
{
		#ifndef TEST_MODULE
				uint16_t frame_adress 			= get_frame_adress(mem_number, frame_number);
				uint16_t frame_size =	get_frame_size(mem_number, frame_number);
				my_memory.get_memory((void*)&FRAME_HEADER, frame_size, frame_adress);
		#endif
	#ifdef TEST_MODULE
		//memcpy((void*)(&FRAME + (frame_number*20)),temp_mem,20);//
		 TEST_get_frame(frame_number);
	#endif

}
//================================================================================
void auto_start_next_frame()
{
	my_player.frame ++; // 
			
			#ifdef TEST_MODULE
					if(my_player.frame > FRAME_IN_TEST_SCEN - 1) my_player.frame = 0;
			#endif 
			PREVIOUS_FRAME = FRAME;
			fill_frame( my_player.mem,  my_player.frame);
			player_start_mem( my_player.mem,  my_player.frame,	 1);
	
	#ifndef MASTER 
			//pin_toggle(PIN_DIGIT_EN_1);
			//pin_toggle(PIN_DIGIT_EN_2);
			//pin_toggle(PIN_DIGIT_EN_3);
			//pin_toggle(PIN_DIGIT_EN_4);
	#endif
}
//================================================================================
void player_play_mem(uint16_t mem_number, uint8_t mem_frame,	uint8_t rate) 
{
		#ifndef TEST_MODULE
			fill_mem_header(mem_number);// 
			fill_playback_header();			//
		#endif
	
		fill_frame( mem_number,  mem_frame);
		player_start_mem( mem_number,  mem_frame,	 rate);

}
//================================================================================
//
void player_start_mem(uint16_t mem_number, uint8_t mem_frame,	int8_t rate) 
{
		
			//set_playback( mem_number,  mem_frame,	 rate);
	// 
		//my_player.rate 								= rate;
		my_player.mem 								= mem_number;
		my_player.frame 							= mem_frame;
		my_player.state								= PLAYBACK_ENABLED; 	// 
		my_player.frame_time_begine 	= systick_timer.tics; // 
		my_player.crosfade						= 0;
	
		

}
static void crossfade_correct(uint8_t new_crossfade)
{
		//uint32_t new_crossfade = (((float)(systick_timer.tics  - my_player.frame_time_begine) /((float)FRAME.crostime*10))*255);

		my_player.frame_time_begine = systick_timer.tics - ((float)FRAME.crostime*10) *((float)new_crossfade / 255);

		my_player.crosfade	= new_crossfade;
}
//================================================================================
void player_synchro(uint16_t mem_number, uint8_t mem_frame,	int8_t rate, uint8_t crossfade) 
{
		if(my_player.mem != mem_number || my_player.frame != mem_frame || my_player.state != PLAYBACK_ENABLED)
		{
			player_play_mem ( mem_number,  mem_frame,	 crossfade) ;
		}
		if( crossfade > my_player.crosfade + DOPUSK|| crossfade < my_player.crosfade - DOPUSK)
		{
				crossfade_correct(crossfade);
		}
			pin_toggle(PIN_DIGIT_EN_1);
			pin_toggle(PIN_DIGIT_EN_2);
			pin_toggle(PIN_DIGIT_EN_3);
			pin_toggle(PIN_DIGIT_EN_4);
}
//================================================================================
//
void fill_playback_header()
{
		my_memory.get_memory((void*)&PLAYBACK_HEADER, sizeof(PLAYBACK_HEADER),  my_memory.my_scen_header.start_adress);
}
//================================================================================
void player_stop_mem()
{
		my_player.state				= PLAYBACK_DISABLED; 	// 
#ifndef MASTER
		for(uint8_t i = 0; i < LED_NUMBER ; i ++)
		{
			my_map.my_leds[i].red 	= 0;
			my_map.my_leds[i].green = 0;
			my_map.my_leds[i].blue 	= 0;
		}
#endif
#ifdef MASTER
		//#ifdef M/StopCmd (RF_ADDRESS_BROADCAST);
		//#endif 
		rf_sendStopCmd(RF_ADDRESS_BROADCAST);
#endif
}
//================================================================================
static uint8_t holdtime_calc()
{
			uint32_t new_holdfade = (((float)(systick_timer.tics  - my_player.frame_time_begine) /((float)FRAME.holdtime*10))*255);
			if( new_holdfade > my_player.hold + 1)
			{
					my_player.hold = new_holdfade;
					if(new_holdfade > 0xFF)// 
					{
							auto_start_next_frame(); // 
							my_player.crosfade	= 0;
							my_player.hold			= 0;
							synchro_flag 				= 0;
					}
					return 0;
			}
		 return 1;
}

//================================================================================
/*

*/
static uint8_t crosfade_calc()
{
	#ifdef MASTER
	if(my_player.crosfade > SYNCHRO_VALUE&& synchro_flag == 0)// 
	{
				rf_sendStartCmd (RF_ADDRESS_BROADCAST, my_player.mem, my_player.frame,0, my_player.crosfade);// �������� ������� ������� �����
				synchro_flag = 1;
	}
	#endif
	
	uint32_t new_crossfade = (((float)(systick_timer.tics  - my_player.frame_time_begine) /((float)FRAME.crostime*10))*255);
		if( new_crossfade > (my_player.crosfade + 1))
		{
				my_player.crosfade = new_crossfade;
			 
					if(new_crossfade > 0xFF)// 
					{
							
							my_player.crosfade = 0xFF;
							return holdtime_calc();	
					}
				return 0;
		}
		else return 1;
}
//================================================================================
// 
void payer_handler(void)
{
		if(my_player.state	== PLAYBACK_DISABLED) return;
		if(crosfade_calc()!=0) return; // 
		#ifndef MASTER 
		set_led_state();
		#endif
}

