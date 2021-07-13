#ifndef _RF_PARSER_H_
#define _RF_PARSER_H_

#define SELF_ADDRESS										11

#define RF_ADDRESS_BROADCAST						((uint16_t)0xffff)

#define RF_FREQ_CHANNEL(channel)				((uint32_t)(SERVICE_FREQUENCY + (((- 5 + channel) * 2500000))))
#define RF_MAX_CHANNEL									10
#define RF_BINDING_REPEAT								5

#define RF_CMD_OPCODE_REGISTER					((uint8_t)0x01)
#define RF_CMD_OPCODE_START							((uint8_t)0x03)
#define RF_CMD_OPCODE_STOP							((uint8_t)0x04)
#define RF_CMD_BIND											((uint8_t)0x05)

#define RF_PACKET_HEADER_LEN						7
#define BIND_RETRY_THRESHOLD                        50
#define TRANSCEIVER_IDLE_TIME_THRESHOLD             (1000 * 5 * 60)
#pragma pack (push, 1)

typedef struct
{
	uint8_t head1;
	uint8_t head2;
	uint8_t head3;
	uint16_t addr;
	uint8_t cmd;
	uint8_t dataLen;
	uint8_t data[];
} rf_packet_t;

typedef struct
{
	uint16_t mem;
	uint16_t scene;
	int8_t rate;
	uint8_t crossfade;
} rf_packet_start_t;

#pragma pack (pop)

void rf_task (void);
void rf_parse (uint8_t *data, uint8_t length);
void rf_sendStartCmd (uint16_t addr, uint16_t mem, uint16_t scene, int8_t rate, uint8_t crossFade);
void rf_sendStopCmd (uint16_t addr);
void rf_init (void);
void rf_binding (uint8_t ch);
void rf_set_channel (uint8_t ch);

#endif /* _RF_PARSER_H_ */
