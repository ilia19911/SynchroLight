// The MIT License (MIT)
//
// Copyright (c) 2015 Aleksandr Aleshin <silencer@quadrius.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.



//#include "ws2812b.h"
#include "include.h"

//------------------------------------------------------------
// Internal
		uint8_t								static_treset_buffer[TRESET]; //����� � ������, ������� �������. ����� ��������� ������� �������, �� ������ �������
		
//------------------------------------------------------------

void ws2812_init_univerces(void)
{
		#ifdef WS2812_3
		init_w2812_TIM2_1_struct();
		ws2812b_Init(&my_ws2812_3,70,105);
	#endif
		
	#ifdef WS2812_1
			init_w2812_TIM2_3_struct();
			ws2812b_Init(&my_ws2812_1,0,35);
	#endif
	#ifdef WS2812_2
			init_w2812_TIM2_4_struct();
			ws2812b_Init(&my_ws2812_2,35,70);
	#endif
	
	//delay_ms(100);
	//DMA1_Channel5->CCR = 0x00003193;
	//DMA1_Channel5->CCR  |=  DMA_CCR5_DIR; 
}
//================================================================================
void set_empty_DMA_buffer(ws2812 *this_ws2812)
{
	if(this_ws2812->empty_first_buffer )//���� �������� ������ ��������
	{
		this_ws2812->empty_first_buffer = 0;
		this_ws2812->buff_pointer = this_ws2812->BUF_DMA1;
	}
	else
	{
		this_ws2812->empty_first_buffer = 1;
		this_ws2812->buff_pointer = this_ws2812->BUF_DMA2;
	}
	this_ws2812->buff32_pointer =(uint32_t)this_ws2812->buff_pointer;
}
//================================================================================
void start_ws2812(ws2812 *this_ws2812)
{
		this_ws2812->size 								= DMA_BUF_LEN;
		this_ws2812->empty_first_buffer		= 1;
		this_ws2812->offset 							= 0;
		
		this_ws2812->state = First_translation; 
	
		//this_ws2812->empty_first_buffer		= 1;
		set_empty_DMA_buffer(this_ws2812);
}
//================================================================================
static void start_DMA(ws2812 *this_ws2812)
{
		TIM_Cmd(this_ws2812->TIMER, ENABLE);
		DMA_Cmd(this_ws2812->DMA_Channel, ENABLE);

		TIM_DMACmd(this_ws2812->TIMER, this_ws2812->timer_dma_cc, ENABLE);
}
//================================================================================
static void stop_DMA(ws2812 *this_ws2812)
{
		TIM_Cmd(this_ws2812->TIMER, DISABLE);
		DMA_Cmd(this_ws2812->DMA_Channel, DISABLE);

		TIM_DMACmd(this_ws2812->TIMER, this_ws2812->timer_dma_cc, DISABLE);
}
//================================================================================
void triple_defination(ws2812 *this_ws2812)
{
	 if(this_ws2812 == &my_ws2812_1)
	{
				stop_DMA(this_ws2812);
				start_DMA(&my_ws2812_2);
	}
	if(this_ws2812 == &my_ws2812_2)
	{
				stop_DMA(this_ws2812);
				start_DMA(&my_ws2812_3);
  
	}
	if(this_ws2812 == &my_ws2812_3)
	{
				stop_DMA(this_ws2812);
				start_DMA(&my_ws2812_1);
	}
}
//================================================================================
void convert_byte_to_bin(uint8_t source, uint8_t *output)
{
	for(uint8_t i = 0; i <8; i ++)
	{
			if( (source >> (7 - i)) & 1)
					output[i] = HIGH;
			else
					output[i] = LOW;
	}
}
//================================================================================
uint16_t convert_led_to_bin(ws2812 *this_ws2812, uint16_t offset, uint16_t led_number, uint8_t *output)
{
	uint16_t counter = 0;
	
	for(uint8_t i = 0; i < led_number; i ++)
	{
				convert_byte_to_bin( this_ws2812->my_led[i + offset].red,				output + 8 +  (i * 24) );
				convert_byte_to_bin( this_ws2812->my_led[i + offset].green,				output + 16 + (i * 24) );
				convert_byte_to_bin( this_ws2812->my_led[i + offset].blue,				output + 0 +  (i * 24) );
				counter += 24;
	}

	return counter;
}
//================================================================================
uint16_t set_binary_data(ws2812 *this_ws2812)
{
	uint16_t leds_to_send			= 0;
	uint16_t byte_to_DMA			= 0;
	
	uint16_t len_number = this_ws2812->stop_led_number - this_ws2812->start_led_number;
	
	leds_to_send = MIN( len_number  - (this_ws2812->offset - this_ws2812->start_led_number) ,  MAX_LED_TO_DMA);

	byte_to_DMA = convert_led_to_bin(this_ws2812, this_ws2812->offset , leds_to_send, this_ws2812->buff_pointer);
	
	if(leds_to_send + (this_ws2812->offset - this_ws2812->start_led_number)  >= len_number)// ������ ���� ����� ���� ��� ��������� �����. ������(>) - ��� ������ ������ �� ����
	{
			this_ws2812->offset  = this_ws2812->start_led_number;
			this_ws2812->state = Last_translation;
	}
	else
			this_ws2812->offset  += leds_to_send;

	return byte_to_DMA;
}
//================================================================================
void led_handler(ws2812 *this_ws2812)
{
	switch(this_ws2812->state)
	{
		case First_translation:
				set_empty_DMA_buffer(this_ws2812);
				this_ws2812->size = set_binary_data(this_ws2812);
		break;
		case Last_translation:
				this_ws2812->state = Treset;
				this_ws2812->buff_pointer = (this_ws2812->treset_buffer);
				this_ws2812->buff32_pointer =(uint32_t)this_ws2812->buff_pointer;
				this_ws2812->size = TRESET;
		break;
		case Treset:
				triple_defination(this_ws2812);
				this_ws2812->state = First_translation;
				set_empty_DMA_buffer(this_ws2812);
				//this_ws2812->offset = 0;
				this_ws2812->size = set_binary_data(this_ws2812);
		break;
		default: 
			this_ws2812->state = Treset; 
		break;
	}
	//������� ��� ���� ����� ����� ����� ������������, dma ����������������
		DMA1_Channel5->CCR = 0x00003193;
		DMA1_Channel5->CCR  |=  DMA_CCR5_DIR; 
	// ����� ������� 
}
//================================================================================
void ws2812b_Init(ws2812  *this_ws2812, uint16_t start_led, uint16_t stop_led)
{

	 // ws2812 *pointer = this_ws2812;
		GPIO_InitTypeDef 						GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef 		TIM_TimeBaseInitStruct;
		TIM_OCInitTypeDef 					TIM_OCInitStruct;
		DMA_InitTypeDef 						DMA_InitStruct;
		NVIC_InitTypeDef 						NVIC_InitStruct;

		memset(static_treset_buffer, 0, TRESET);
	

    // Initialize GPIO pin
    GPIO_StructInit(&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = this_ws2812->GPIO_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(this_ws2812->GPIO, &GPIO_InitStruct);

    // Initialize timer clock
		TIM_TimeBaseInitStruct.TIM_Prescaler				=  0;
    TIM_TimeBaseInitStruct.TIM_CounterMode			= TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period						= WS2812B_PERIOD - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision		= TIM_CKD_DIV1;

    TIM_TimeBaseInit(this_ws2812->TIMER, &TIM_TimeBaseInitStruct);

    // Initialize timer PWM
    TIM_OCStructInit(&TIM_OCInitStruct);

    TIM_OCInitStruct.TIM_OCMode				= TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState 	= TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse				= 0;
    TIM_OCInitStruct.TIM_OCPolarity 	= TIM_OCPolarity_High;

    this_ws2812->TIM_OCInit(this_ws2812->TIMER, &TIM_OCInitStruct);
    this_ws2812->TIM_OCPreloadConfig(this_ws2812->TIMER, TIM_OCPreload_Enable);

    // Initialize DMA channel
    DMA_StructInit(&DMA_InitStruct);

    DMA_InitStruct.DMA_PeripheralBaseAddr					= (uint32_t) this_ws2812->timer_dma_ccr;
    DMA_InitStruct.DMA_MemoryBaseAddr							= (uint32_t) this_ws2812->BUF_DMA1;
		DMA_InitStruct.DMA_DIR												= DMA_DIR_PeripheralDST;
		DMA_InitStruct.DMA_BufferSize									= sizeof(this_ws2812->BUF_DMA1);
		DMA_InitStruct.DMA_PeripheralInc							= DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc									= DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize					= DMA_PeripheralDataSize_HalfWord;
		DMA_InitStruct.DMA_MemoryDataSize 						= DMA_MemoryDataSize_Byte;
		DMA_InitStruct.DMA_Mode												= DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority										= DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_M2M												= DMA_M2M_Disable;

    DMA_Init(this_ws2812->DMA_Channel, &DMA_InitStruct);
       
		
    // Turn on timer DMA requests
    TIM_DMACmd(this_ws2812->TIMER, this_ws2812->timer_dma_cc, ENABLE);

    // Initialize DMA interrupt  
    NVIC_InitStruct.NVIC_IRQChannel =this_ws2812->DMA_IRQ_NUMBER;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = this_ws2812->NVIC_prioritet;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = WS2812B_IRQ_SUBPRIO;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStruct);

    // Enable DMA interrupt
		DMA_ITConfig(this_ws2812->DMA_Channel, DMA_IT_TC, ENABLE);
		
		
		this_ws2812->start_led_number	  = start_led;
		this_ws2812->stop_led_number 	  = stop_led;
		this_ws2812->my_led 			 			= static_led;// ����� �����������
		this_ws2812->treset_buffer 		  = static_treset_buffer;
		this_ws2812->offset  						= this_ws2812->start_led_number;
		
		start_ws2812(this_ws2812);
		start_DMA(this_ws2812);
}

//================================================================================
#ifdef WS2812_1

//================================================================================
void init_w2812_TIM2_3_struct(void)
{
		my_ws2812_1.DMA							=	WS2812B_DMA;
		my_ws2812_1.DMA_Channel					=	WS2812B_DMA_CHANNEL;
		my_ws2812_1.IT_TC 						=	WS2812B_DMA_IT_TC;
 		my_ws2812_1.DMA_HANDLER					=	WS2812B_DMA_HANDLER;
		my_ws2812_1.DMA_IRQ_NUMBER				=	DMA1_Channel1_IRQn;
		my_ws2812_1.GPIO						=	WS2812B_GPIO;
		my_ws2812_1.GPIO_PIN					=	WS2812B_GPIO_PIN;
		
		my_ws2812_1.TIMER						=	TIM2;
		my_ws2812_1.timer_dma_cc				=	TIM_DMA_CC3;
			
		my_ws2812_1.TIM_OCInit					=	TIM_OC3Init;
		my_ws2812_1.TIM_OCPreloadConfig			=	TIM_OC3PreloadConfig;
		
		my_ws2812_1.timer_dma_ccr				=	&TIM2->CCR3;
		my_ws2812_1.NVIC_prioritet				=	2;
}
//================================================================================
void DMA1_Channel1_IRQHandler(void)
{
			//ENTER_CRITICAL_SECTION();
					//DMA_ClearITPendingBit(WS2812B_DMA_IT_TC);
				WS2812B_DMA->IFCR = WS2812B_DMA_IT_TC;
				/* Disable the selected DMAy Channelx */
				WS2812B_DMA_CHANNEL->CCR &= (uint16_t)(~DMA_CCR1_EN);

				//WS2812B_DMA_CHANNEL ->CMAR = (uint32_t)my_ws2812_1.buff_pointer;
				WS2812B_DMA_CHANNEL ->CMAR = my_ws2812_1.buff32_pointer;
				WS2812B_DMA_CHANNEL->CNDTR = my_ws2812_1.size;
				/* Enable the selected DMAy Channelx */
				//TIM2->CCR3  = my_ws2812_1.buff_pointer[0];
				WS2812B_DMA_CHANNEL->CCR |= DMA_CCR1_EN;
				led_handler(&my_ws2812_1);
				//my_ws2812_1.flag = 1;
			//LEAVE_CRITICAL_SECTION();
} 
#endif

#ifdef WS2812_2

//=============================================================================
		void init_w2812_TIM2_4_struct(void)
		{
		
				my_ws2812_2.DMA 						= DMA1;
				my_ws2812_2.DMA_Channel 				= DMA1_Channel7;
				my_ws2812_2.IT_TC 						= DMA1_IT_TC7;
				my_ws2812_2.DMA_HANDLER 				= DMA1_Channel7_IRQHandler;
				my_ws2812_2.DMA_IRQ_NUMBER 				= DMA1_Channel7_IRQn;
				my_ws2812_2.GPIO						= GPIOA;
				my_ws2812_2.GPIO_PIN					= GPIO_Pin_3;
			
				my_ws2812_2.TIMER						=	TIM2;
				my_ws2812_2.timer_dma_cc				= TIM_DMA_CC4;
									
				my_ws2812_2.TIM_OCInit					=	TIM_OC4Init;
				my_ws2812_2.TIM_OCPreloadConfig = TIM_OC4PreloadConfig;
									
				my_ws2812_2.timer_dma_ccr				=	&TIM2->CCR4;
				my_ws2812_2.NVIC_prioritet				= 2;
		}
		//================================================================================
		void DMA1_Channel7_IRQHandler(void)
		{
			//ENTER_CRITICAL_SECTION();
				DMA1->IFCR = DMA1_IT_TC7;
		
				/* Disable the selected DMAy Channelx */
				DMA1_Channel7->CCR &= (uint16_t)(~DMA_CCR7_EN);
		
				DMA1_Channel7 ->CMAR = my_ws2812_2.buff32_pointer;
				DMA1_Channel7->CNDTR = my_ws2812_2.size;
				/* Enable the selected DMAy Channelx */
				//TIM2->CCR4  = my_ws2812_2.buff_pointer[0];
				DMA1_Channel7->CCR |= DMA_CCR7_EN;
				led_handler(&my_ws2812_2);
					
				//my_ws2812_2.flag = 1;
		//	LEAVE_CRITICAL_SECTION();
		}  
//================================================================================
#endif

#ifdef WS2812_3

//================================================================================
		void init_w2812_TIM2_1_struct(void)
		{
		
				my_ws2812_3.DMA 							= DMA1;
				my_ws2812_3.DMA_Channel 			= DMA1_Channel5;
				my_ws2812_3.IT_TC 						= DMA1_IT_TC5;
				my_ws2812_3.DMA_HANDLER 			= DMA1_Channel5_IRQHandler;
				my_ws2812_3.DMA_IRQ_NUMBER 		= DMA1_Channel5_IRQn;
				my_ws2812_3.GPIO							= GPIOA;
				my_ws2812_3.GPIO_PIN					= GPIO_Pin_0;
				
				my_ws2812_3.TIMER							= TIM2;
				my_ws2812_3.timer_dma_cc			= TIM_DMA_CC1;
									
				my_ws2812_3.TIM_OCInit				= TIM_OC1Init;
				my_ws2812_3.TIM_OCPreloadConfig 		= TIM_OC1PreloadConfig;
									
				my_ws2812_3.timer_dma_ccr			=&TIM2->CCR1;
				my_ws2812_3.NVIC_prioritet				= 2;
		}
//=============================================================================
		void DMA1_Channel5_IRQHandler(void)
		{
			//ENTER_CRITICAL_SECTION();
				DMA1->IFCR = DMA1_IT_TC5;
				
				/* Disable the selected DMAy Channelx */
				DMA1_Channel5->CCR &= (uint16_t)(~DMA_CCR5_EN);
				
				DMA1_Channel5 ->CMAR = my_ws2812_3.buff32_pointer;
				DMA1_Channel5->CNDTR = my_ws2812_3.size;
				/* Enable the selected DMAy Channelx */
				DMA1_Channel5->CCR |= DMA_CCR5_EN;
				led_handler(&my_ws2812_3);
				
				//my_ws2812_3.flag = 1;
		//	LEAVE_CRITICAL_SECTION();
		}
//================================================================================

#endif


