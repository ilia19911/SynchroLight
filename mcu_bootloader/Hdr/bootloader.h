#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include "bootconf.h"

#define BOOTLOADER_CMD_GETVERSION					((uint8_t)0x00)
#define BOOTLOADER_CMD_SENDPROMPT					((uint8_t)0x01)
#define BOOTLOADER_CMD_SENDDATA						((uint8_t)0x02)
#define BOOTLOADER_CMD_SENDCHKSUM					((uint8_t)0x03)

#define BOOTLOADER_HEADER_SIZE						5

typedef void (*pFunction)(void);

typedef enum
{
	BOOTLOADER_CONFIRM_ERROR = 0,
	BOOTLOADER_CONFIRM_UART = 1,
	BOOTLOADER_CONFIRM_SX1276 = 2
} bootloader_confirmation_t;

typedef enum
{
	BOOTLOADER_RCV_IDLE = 0,
	BOOTLOADER_RCV_HEADER1 = 1,
	BOOTLOADER_RCV_HEADER2 = 2,
	BOOTLOADER_RCV_HEADER3 = 3,
	BOOTLOADER_RCV_LEN = 4,
	BOOTLOADER_RCV_LEN_INV = 5,
	BOOTLOADER_RCV_ADDR1 = 6,
	BOOTLOADER_RCV_ADDR2 = 7,
	BOOTLOADER_RCV_DATA = 8,
	BOOTLOADER_RCV_READY = 9
} bootloader_rcv_state_t;

typedef struct
{
	struct {
		uart_t *uart;
		bootloader_rcv_state_t state;
		uint8_t buffer[256];
		uint8_t lenToRcv;
		uint8_t packetLen;
		uint8_t *pnt;
	} receiver;
	struct {
		uint32_t offset;
		uint32_t prevOffset;
		uint32_t crc32;
		uint32_t totalLength;
		uint32_t crc32Calculated;
	} firmware;
	timeout_t startup;
} bootloader_t;

#pragma pack (push, 1)

typedef struct
{
	uint8_t prefix1;
	uint8_t prefix2;
	uint8_t prefix3;
	uint8_t len;
	uint8_t nlen;
	uint8_t cmd;
	uint16_t address;
	uint8_t data[];
} bootloader_msg_t;


typedef struct
{
	uint8_t version_major;
	uint8_t version_minor;
} bootloader_version_t;

typedef struct
{
    uint32_t length;
    uint32_t crc32;
} bootloader_file_prompt_t;

typedef struct
{
    uint8_t len;
    uint32_t offset;
    uint8_t data[];
} bootloader_data_t;

#pragma pack (pop)

void bootloader_init (uart_t *uart);
void bootloader_task (void);

#endif /* _BOOTLOADER_H_ */
