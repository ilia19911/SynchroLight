
#ifndef _LOW_LEVEL0_MASTER_V2_H_
#define _LOW_LEVEL0_MASTER_V2_H_

/* ======================================================================	*/
/* В данном файле описываются в формета pin_macro все ноги контроллера		*/
/* Которые будут использоваться в проекте.																*/
/* Также здесь объявлены функции для стартовой инициализации							*/
/* ======================================================================	*/

// USART для общения с отладчиком по уарту
#define PIN_UART1_TX     		    	A, 9,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART1_RX							A,10,H,INPUT_FLOATING,SPEED_50MHZ

// USART для общения с ГПС модулем
#define PIN_UART2_TX          		A, 2,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART2_RX          		A, 3,H,INPUT_FLOATING,SPEED_50MHZ

// USART для общения с нагрузкой по DMX протоколу
#define PIN_UART3_TX          		B,10,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART3_RX          		B,11,H,INPUT_FLOATING,SPEED_50MHZ

// SPI для общения с трансиверами
#define PIN_TRC_RESET							B, 0,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_TRC_SCK						A, 5,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_TRC_MOSI					A, 7,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_TRC_MISO					A, 6,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_SPI_TRC_CS						A, 4,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// SPI для общения с флэшкой
#define PIN_SPI_FLASH_SCK					B,13,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_FLASH_MOSI				B,15,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_FLASH_MISO				B,14,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_SPI_FLASH_SD_CS				A,12,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_FLASH_AT_CS				B,12,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_FLASH_CARD_DET				A, 8,H,INPUT_PULL_UP,SPEED_50MHZ					// Теперь у нас этот пин будет использоваться как генератор частоты на трансивер
//#define PIN_MCO_OUT							A, 8,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ

// Кнопки
#define PIN_BUTTON_1							B, 9,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_BUTTON_2							C,13,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_BUTTON_3							C,14,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_BUTTON_4							C,15,H,INPUT_PULL_UP,SPEED_50MHZ

// Выходы на семисгментный индикатор
#define PIN_SEG_A									B, 7,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SEG_B									A, 1,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SEG_C									A, 0,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SEG_D									B, 5,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SEG_E									B, 1,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SEG_F									A,11,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SEG_G									B, 6,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// Выходы на включение сегментов
#define PIN_DIGIT_EN_1						B, 8,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_DIGIT_EN_2						A,15,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_DIGIT_EN_3						B, 3,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_DIGIT_EN_4						B, 4,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

#define uart_debug					uart1

void initLowLevel (void);
void transceiverTask (void);

#endif /* _LOW_LEVEL0_H_ */
