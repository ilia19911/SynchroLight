#include "include.h"

//static void rf_parse (uint8_t *data, uint8_t length);
static const uint8_t rfheader[3] = {0xcd, 0x16, 0xbb};

static uint8_t txBuff[64];
static uni_eeprom_t settings;

void rf_init (void)
{
	settings.in.data = (void*)&channel;
	settings.in.data_size = sizeof (uint16_t);
	settings.in.page_number = 127;
	settings.in.auto_page_erase_fl = 0;
	
	uni_eeprom_init (&settings);
	uni_eeprom_init_and_get (&settings);
	
	// Если тут хз что - ставим 5 канал и пересохраняем
	if (channel > 10)
	{
		channel = 5;
		uni_eeprom_data_save (&settings);
	}		
	
	// У нас есть номер канала -- переключаемся на него
	sx1276_LoRa_SetFrequency (&transc, RF_FREQ_CHANNEL(channel));
	delay_ms (100);
	sx1276_LoRa_startRx (&transc);
}

void rf_task (void)
{
	// Обновим состояние трансивера
	if (software_timer (&transc_update)) sx1276_LoRa_updateState (&transc);
	
	// Если есть принятые данные - обрабатываем их
	if (transc.receiver.readyRead)
	{
		#ifdef MASTER
			
		#else
			rf_parse (transc.receiver.rxBuffer, transc.receiver.bytesReceived);
			#ifdef SLAVE_RETRANSLATOR
			sx1276_LoRa_sendPacket(&transc, transc.receiver.rxBuffer, transc.receiver.bytesReceived);
			delay_ms(250);
			#endif /* SLAVE_RETRANSLATOR */
		#endif /* MASTER */
		transc.receiver.readyRead = 0;
	}
}

void rf_parse (uint8_t *data, uint8_t length)
{
	rf_packet_t *pck = (void*)data;
	rf_packet_start_t *st = (void*)pck->data;
	// Проверка хидера
	if (memcmp (data, rfheader, sizeof (rfheader)) != 0) 
	{
		dprintf ("Receive unsupported message\r\n");
		return;
	}
	
	// Проверка адреса
	if ((pck->addr != SELF_ADDRESS) && (pck->addr != RF_ADDRESS_BROADCAST))
	{
		dprintf ("Receive misstake message\r\n");
		return;
	}
	
	// Адрес наш. Обрабатываем-с
	switch (pck->cmd)
	{
		case RF_CMD_OPCODE_REGISTER:
			
			break;
		
		case RF_CMD_OPCODE_START:
			player_synchro(st->mem, st->scene, st->rate, st->crossfade); 
			
			break;
		
		case RF_CMD_OPCODE_STOP:
			player_stop_mem();		
			break;
		
		case RF_CMD_BIND:
			// Команда смены канала. Вызываем если канал новый
			if (pck->data[0] != channel) rf_set_channel (pck->data[0]);
			break;
		
		default: break;
	}
}

void rf_sendStartCmd (uint16_t addr, uint16_t mem, uint16_t scene, int8_t rate, uint8_t crossFade)
{
	rf_packet_t *pck = (void*)txBuff;
	rf_packet_start_t *start = (void*)pck->data;
	
	// Запихиваем хидер
	memcpy (txBuff, rfheader, sizeof (rfheader));
	
	pck->addr = addr;
	pck->cmd = RF_CMD_OPCODE_START;
	pck->dataLen = sizeof (rf_packet_start_t);
	
	start->mem = mem;
	start->scene = scene;
	start->rate = rate;
	start->crossfade = crossFade;
	
	sx1276_LoRa_sendPacket (&transc, txBuff, RF_PACKET_HEADER_LEN + pck->dataLen);
}

void rf_sendStopCmd (uint16_t addr)
{
	rf_packet_t *pck = (void*)txBuff;
	
	// Запихиваем хидер
	memcpy (txBuff, rfheader, sizeof (rfheader));
	
	pck->addr = addr;
	pck->cmd = RF_CMD_OPCODE_STOP;
	pck->dataLen = 0;
	
	sx1276_LoRa_sendPacket (&transc, txBuff, RF_PACKET_HEADER_LEN + pck->dataLen);	
}

void rf_binding (uint8_t ch)
{
	rf_packet_t *pck = (void*)txBuff;
	
	// Запихиваем хидер
	memcpy (txBuff, rfheader, sizeof (rfheader));
	
	// Собираем посылку
	pck->addr = RF_ADDRESS_BROADCAST;
	pck->dataLen = 1;
	pck->cmd = RF_CMD_BIND;
	pck->data[0] = ch;
	
	// И теперь по всем частотам разошлем посылку
	for (uint8_t k = 0; k < RF_MAX_CHANNEL; k++)
	{
		sx1276_LoRa_SetFrequency (&transc, RF_FREQ_CHANNEL(k));
		delay_ms (20);
		
		for (uint8_t j = 0; j < RF_BINDING_REPEAT; j++)
		{
			sx1276_LoRa_sendPacket (&transc, txBuff, RF_PACKET_HEADER_LEN + pck->dataLen);
			while (transc.Control.busy) 
			{
				dislay_Handler ();
				transceiverTask ();
			}
			delay_ms (50);
		}
	}
	
	// Всех оповестили -- можно и самим на частоту новую переходить
	rf_set_channel (ch);
}

void rf_set_channel (uint8_t ch)
{
	sx1276_LoRa_SetFrequency (&transc, RF_FREQ_CHANNEL(ch));
	delay_ms (100);
	sx1276_LoRa_startRx (&transc);
	
	// Сохраним новое значение канала
	channel = ch;
	uni_eeprom_data_save (&settings);
}
