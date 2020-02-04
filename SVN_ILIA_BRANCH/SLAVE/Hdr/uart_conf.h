#ifndef _UART_CONF_H_
#define _UART_CONF_H_

/* ======================================================================	*/
/* Файл конфигурации U(S)ART'ов. 																					*/
/* Здесь задаются размеры буферов ФИФО для каждого												*/
/* ======================================================================	*/

// Буфер для "общения" с логгером/дебагконсолью
#define UART1_TX_BUFFER_SIZE 256
#define UART1_RX_BUFFER_SIZE 256

// Буфер для "общения" с модемом
#define UART2_TX_BUFFER_SIZE 256
#define UART2_RX_BUFFER_SIZE 256

#endif /* _UART_CONF_H_ */
