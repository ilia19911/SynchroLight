#ifndef _VAR_MASTER_H_
#define _VAR_MASTER_H_

/* ======================================================================	*/
/* Тут описываются ВСЕ глобальный переменные проекта.											*/
/* Только тут.																														*/
/* ======================================================================	*/
#define RF_CLOCK_FREQUENCY														32000000
#define SERVICE_FREQUENCY														869000000


#if !defined (MASTER) && !defined (SLAVE)
	#error Please specefy - this device master or slave
#endif
	
#if defined (MASTER) && defined (SLAVE) //Device can't be in both mode - master and slave. Specify only one
	#error  
#endif


#ifdef MAIN

sx1276_LoRa_Modem						transc;
timeout_t 								transc_update;
//
//#ifdef WS2812_1
//		ws2812 							my_ws2812_1;
//#endif
//#ifdef WS2812_2
//		ws2812 							my_ws2812_2;
//#endif
//#ifdef WS2812_3
//		ws2812 							my_ws2812_3;
//#endif
//
//leds 									static_led[LED_NUMBER];

main_memory 							my_memory;
player 									my_player;
//led_map 								my_map;
coverter								my_coverter;



//uint8_t									active = 0;
uint16_t									channel;
#else /* MAIN */

#include "include.h"


#define FRAME								my_player.frame_header
#define PREVIOUS_FRAME			my_player.previous_frame
#define PLAYBACK_HEADER 		my_player.playback_header


extern sx1276_LoRa_Modem transc;
extern timeout_t transc_update;

//#ifdef WS2812_1
//	extern ws2812 				my_ws2812_1;
//#endif
//#ifdef WS2812_2
//	extern ws2812 				my_ws2812_2;
//#endif
//#ifdef WS2812_3
//	extern ws2812 				my_ws2812_3;
//#endif
//

//extern leds 					static_led[LED_NUMBER];
extern main_memory 		my_memory;
extern player 				my_player;
//extern led_map 				my_map;
extern coverter				my_coverter;
//extern uint8_t				active;
extern uint16_t									channel;
#endif /* MAIN */

#endif /* _VAR_H_ */
