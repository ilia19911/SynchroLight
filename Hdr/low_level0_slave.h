#ifndef _LOW_LEVEL0_SLAVE_H_
#define _LOW_LEVEL0_SLAVE_H_

/* ======================================================================	*/
/* В данном файле описываются в формета pin_macro все ноги контроллера		*/
/* Которые будут использоваться в проекте.																*/
/* Также здесь объявлены функции для стартовой инициализации							*/
/* ======================================================================	*/

// USART для общения с отладчиком по уарту
#define PIN_UART1_TX     		    A, 9,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART1_RX				A,10,H,INPUT_FLOATING,SPEED_50MHZ

// USART для общения с ГПС модулем
#define PIN_UART2_TX          		A, 2,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART2_RX          		A, 3,H,INPUT_FLOATING,SPEED_50MHZ

// USART для общения с нагрузкой по DMX протоколу
#define PIN_UART3_TX          		B,10,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART3_RX          		B,11,H,INPUT_FLOATING,SPEED_50MHZ

// SPI для общения с трансиверами
#define PIN_TRC_RESET				B, 0,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_TRC_SCK				A, 5,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_TRC_MOSI			A, 7,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_TRC_MISO			A, 6,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_SPI_TRC_CS				A, 4,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// SPI для общения с флэшкой
#define PIN_SPI_FLASH_SCK			B,13,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_FLASH_MOSI			B,14,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_FLASH_MISO			B,15,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_SPI_FLASH_SD_CS			A,12,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_FLASH_AT_CS			B,12,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// Кнопки
//#define PIN_BUTTON_1				A, 1,H,INPUT_PULL_UP,SPEED_50MHZ
//#define PIN_BUTTON_2				B, 2,H,INPUT_PULL_UP,SPEED_50MHZ
//#define PIN_BUTTON_3				A, 0,H,INPUT_PULL_UP,SPEED_50MHZ
//#define PIN_BUTTON_4				C,15,H,INPUT_PULL_UP,SPEED_50MHZ

// Выходы на двоично-десятичный кодер
//#define PIN_BCD_0					B, 3,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
//#define PIN_BCD_1					B, 4,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
//#define PIN_BCD_2					C,14,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
//#define PIN_BCD_3					C,13,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

// Выходы на включение сегментов
//#define PIN_DIGIT_EN_1				A,11,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
//#define PIN_DIGIT_EN_2				B, 9,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
//#define PIN_DIGIT_EN_3				B, 8,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
//#define PIN_DIGIT_EN_4				A,15,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

#define uart_debug					uart1

void initLowLevel (void);
void transceiverTask (void);
void initTransceiver (void);

#endif /* _LOW_LEVEL0_H_ */
