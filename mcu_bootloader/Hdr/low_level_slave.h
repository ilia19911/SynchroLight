#ifndef _LOW_LEVEL0_H_
#define _LOW_LEVEL0_H_

/* ======================================================================== */
/* В данном файле описываются в формета pin_macro все ноги контроллера		*/
/* Которые будут использоваться в проекте.									*/
/* Также здесь объявлены функции для стартовой инициализации				*/
/* ======================================================================== */

// USART
#define PIN_UART_TX						A, 9,H,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_UART_RX 					A,10,H,INPUT_FLOATING,SPEED_50MHZ

// SPI для общения с трансиверами
#define PIN_SPI_SCK						A, 5,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_MOSI					A, 7,L,ALT_OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_SPI_MISO					A, 6,H,INPUT_PULL_UP,SPEED_50MHZ
#define PIN_SPI_CS						A, 4,L,OUTPUT_PUSH_PULL,SPEED_50MHZ
#define PIN_TR_RESET					B, 0,L,OUTPUT_PUSH_PULL,SPEED_50MHZ

void initLowLevel (void);

#endif /* _LOW_LEVEL0_H_ */
