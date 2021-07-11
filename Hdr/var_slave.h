#ifndef _VAR_SLAVE_H_
#define _VAR_SLAVE_H_

/* ======================================================================	*/
/* Тут описываются ВСЕ глобальный переменные проекта.											*/
/* Только тут.																														*/
/* ======================================================================	*/
#define RF_CLOCK_FREQUENCY													32000000
#define SERVICE_FREQUENCY														869000000




#if !defined (MASTER) && !defined (SLAVE)
	#error Please specefy - this device master or slave
#endif
	
#if defined (MASTER) && defined (SLAVE) //#error Device can't be in both mode - master and slave. Specify only one
	
#endif



//#define LED_RGB
//#define LED_RBG
//#define LED_GBR
//#define LED_GRB
//#define LED_BGR
#define LED_BRG  // определение последовательности цветов в ленте . нужно сделать переменной позже

#define RED_LED_POW 0.99f //пропорциональное изменение мощности красного цвета
#define GREEN_LED_POW 0.6517f * 0.80f //пропорциональное изменение мощности зеленого цвета
#define BLUE_LED_POW 0.6455317f * 0.70f //пропорциональное изменение мощности синего цвета

#ifdef MAIN
sx1276_LoRa_Modem 			transc;
timeout_t 							transc_update;

#ifdef WS2812_1
		ws2812 									my_ws2812_1;
#endif
#ifdef WS2812_2
		ws2812 									my_ws2812_2;
#endif
#ifdef WS2812_3
		ws2812 									my_ws2812_3;
#endif

leds 									static_led[LED_NUMBER];

main_memory 							my_memory;
player 										my_player;
led_map 									my_map;
coverter									my_coverter;
uint16_t									channel;
#else /* MAIN */

#include "include.h"
extern sx1276_LoRa_Modem transc;
extern timeout_t transc_update;

#ifdef WS2812_1
	extern ws2812 				my_ws2812_1;
#endif
#ifdef WS2812_2
	extern ws2812 				my_ws2812_2;
#endif
#ifdef WS2812_3
	extern ws2812 				my_ws2812_3;
#endif


#define FRAME								my_player.frame_header
#define PREVIOUS_FRAME			my_player.previous_frame
#define PLAYBACK_HEADER 		my_player.playback_header

extern leds 						static_led[LED_NUMBER];
extern main_memory			my_memory;
extern player 					my_player;
extern led_map 					my_map;
extern coverter					my_coverter;
extern uint16_t					channel;
#endif /* MAIN */

#endif /* _VAR_H_ */
