// The MIT License (MIT)
//
// xyN


#ifndef __WS2812B_H
#define __WS2812B_H

#include <misc.h>
#include <stdint.h>
#include <string.h>


#define WS2812_1								//раскаментировать чтобы включить первый поток
#define WS2812_2								//раскаментировать чтобы включить второй поток
#define WS2812_3								//раскаментировать чтобы включить третий поток


#define WS2812B_GPIO            GPIOA
#define WS2812B_GPIO_PIN        GPIO_Pin_2

#define WS2812B_TIM             TIM2
#define WS2812B_TIM_OCINIT      TIM_OC3Init
#define WS2812B_TIM_OCPRELOAD   TIM_OC3PreloadConfig
#define WS2812B_TIM_DMA_CC      TIM_DMA_CC3
#define WS2812B_TIM_DMA_CCR     (WS2812B_TIM->CCR3)

#define WS2812B_DMA             DMA1
#define WS2812B_DMA_CHANNEL     DMA1_Channel1
#define WS2812B_DMA_IRQ         DMA1_Channel1_IRQn

#define WS2812B_DMA_HANDLER     DMA1_Channel1_IRQHandler
#define WS2812B_DMA_IT_TC       DMA1_IT_TC1
#define WS2812B_IRQ_PRIO        0
#define WS2812B_IRQ_SUBPRIO     0


#define WS2812B_FREQUENCY       F_CPU
#define WS2812B_PERIOD          80

#define HIGH										54 // ПЕРИОД ДЛЯ ЛОГИСЕСКОЙ ЕДЕНИЦЫ
#define LOW											26 // ПЕРИОД ДЛЯ ЛОГИСЕСКОГО НУЛЯ
#define TRESET									200 // длина сигнала брейка	

#define LED_NUMBER 							120
#define DMA_BUF_LEN 						400

#define MAX_LED_TO_DMA 					(((DMA_BUF_LEN/8)/3))
#define MIN(a,b)                (((a)<(b))?(a):(b))



enum
{
	First_translation,
	//translation,
	Last_translation,
	Treset
	//Start_translaion
};
#pragma pack(push,2) 
typedef struct 
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}leds;
#pragma pack(pop)



typedef struct 
{
	uint8_t								state; // стате
	uint8_t								*buff_pointer; //смещение
	uint32_t							buff32_pointer; //смещение
	uint8_t								BUF_DMA1 [DMA_BUF_LEN];// буфер dma
	uint8_t								BUF_DMA2 [DMA_BUF_LEN];// буфер dma
	uint8_t								*treset_buffer; //буфер с нулями, нужного размера. Чтобы интерфейс работал быстрее, он создан заранее
			
	leds									*my_led; // буфер светодиодов
	uint16_t							size;
	uint8_t								empty_first_buffer;
	uint16_t							offset;
	//uint8_t								flag;
	uint8_t								start_led_number;
	uint8_t								stop_led_number;
				
				
	GPIO_TypeDef					*GPIO;
	uint16_t							GPIO_PIN;
	TIM_TypeDef						*TIMER;
	DMA_Channel_TypeDef		*DMA_Channel;
	uint16_t 							timer_dma_cc;
	__IO uint16_t 				*timer_dma_ccr;
		
	DMA_TypeDef 					*DMA;
	uint8_t 							DMA_IRQ_NUMBER;
	void (*DMA_HANDLER)		(void);
	uint32_t							IT_TC;
	uint8_t								NVIC_prioritet;
	
	void 									(*TIM_OCInit)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
	void 									(*TIM_OCPreloadConfig)(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
}ws2812;

void ws2812_init_univerces(void);
void ws2812b_Init(ws2812  *this_ws2812, uint16_t start_led, uint16_t stop_led);
void ws2812b_Init_test(ws2812 *this_ws2812, uint16_t start_led, uint16_t stop_led);


void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);

void init_w2812_TIM2_3_struct(void);
void init_w2812_TIM2_4_struct(void);
void init_w2812_TIM2_1_struct(void);

void USBWakeUp_IRQHandler(void);

#endif //__WS2812B_H
