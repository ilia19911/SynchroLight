#include "include.h"

static const uint8_t prefix[3] = {0xb0, 0x3b, 0x7a};

#ifdef BOOTLOADER_MODULE_UART
static void incState (void);
static void resState (void);
static void bootloader_processByte (uint8_t ch);
#endif /* BOOTLOADER_MODULE_UART */

#ifdef BOOTLOADER_MODULE_SX1276
static void radioTask (void);
#endif /* BOOTLOADER_MODULE_SX1276 */

static uint8_t answer[32];
static uint8_t answerLen;

static uint8_t processPacket (uint8_t *data, uint8_t *answer, bootloader_confirmation_t conf);
static void runApplication (void);

bootloader_t bl;

void bootloader_init (uart_t *uart)
{
	int32_t testWord;

	bl.receiver.uart = uart;
	bl.receiver.pnt = bl.receiver.buffer;
	bl.receiver.state = BOOTLOADER_RCV_IDLE;
	software_timer_stop(&bl.startup);

	testWord = *(__IO uint32_t*)BOOTLOADER_APPLICATION_ADDRESS;
	if (testWord != -1)
		software_timer_start(&bl.startup, 2000);
}

void bootloader_task (void)
{
#ifdef BOOTLOADER_MODULE_UART
	int ch;

	// Считываем данные из уарта
	for (;;)
	{
		ch = uart_getc (bl.receiver.uart);
		if (ch < 0) break;
		bootloader_processByte ((uint8_t)ch);
	}

	// Проверка на то, что пакет уже принят
	if (bl.receiver.state == BOOTLOADER_RCV_READY)
	{
		answerLen = processPacket (bl.receiver.buffer, answer, BOOTLOADER_CONFIRM_UART);
		uart_put_block (bl.receiver.uart, answer, answerLen);
	}
#endif

#ifdef BOOTLOADER_MODULE_SX1276
	radioTask ();
#endif /* BOOTLOADER_MODULE_SX1276 */

	// Проверка на готовность переходить в аппликуху
	if (software_timer (&bl.startup)) runApplication ();
}

#ifdef BOOTLOADER_MODULE_UART
static void incState (void)
{
	bl.receiver.state++;
	bl.receiver.pnt++;
}

static void resState (void)
{
	bl.receiver.state = BOOTLOADER_RCV_IDLE;
	bl.receiver.pnt = bl.receiver.buffer;
}

static void bootloader_processByte (uint8_t ch)
{
	*bl.receiver.pnt = ch;

	switch (bl.receiver.state)
	{
	case 0:
		if (ch == prefix[0]) incState ();
		break;

	case 1:
		if (ch == prefix[1]) incState ();
		else resState ();
		break;

	case 2:
		if (ch == prefix[2]) incState ();
		else resState ();
		break;

	case 3:
		bl.receiver.lenToRcv = ch;
		bl.receiver.packetLen = ch;
		incState ();
		break;

	case 4:
		if ((~bl.receiver.lenToRcv & 0xff) == ch) incState ();
		else resState ();
		break;

	case 5:
		incState();
		break;

	case 6:
		incState();
		break;

	case 7:
		if (!bl.receiver.lenToRcv) incState ();
		else
		{
			bl.receiver.lenToRcv--;
			bl.receiver.pnt++;
		}
		break;

	case 8:
		if (crc8 (bl.receiver.buffer + BOOTLOADER_HEADER_SIZE, bl.receiver.packetLen + 3) == ch) incState ();
		else resState ();
		break;

	case 9:
		break;

	}
}
#endif /* BOOTLOADER_MODULE_UART */

uint8_t processPacket (uint8_t *data, uint8_t *answer, bootloader_confirmation_t conf)
{
#ifdef BOOTLOADER_MODULE_UART
	bootloader_msg_t *msg = (void*)data;
	bootloader_msg_t *ans = (void*)answer;
#endif

#ifdef BOOTLOADER_MODULE_SX1276
	bootloader_msg_t *msg = (void*)data;
	bootloader_msg_t *ans = (void*)answer;
	
	// Проверка на совпадение адреса
	if ((msg->address != sx1276address) && (msg->address != BOOTLOADER_SX_ADDRESS_BROADCAST)) return 0;

	ans->address = sx1276address;
#endif

	bootloader_version_t *vers = (void*)ans->data;
	bootloader_file_prompt_t *fprompt = (void*)msg->data;
	bootloader_data_t *fdata = (void*)msg->data;

	uint8_t *crc;
	uint8_t pagesNum;

	memcpy (&ans->prefix1, prefix, sizeof (prefix));
	

	switch (msg->cmd)
	{
	case BOOTLOADER_CMD_GETVERSION:
		// Запрос версии бутлоадера
		ans->len = sizeof (bootloader_version_t);
		ans->nlen = ~ans->len & 0xff;
		ans->cmd = BOOTLOADER_CMD_GETVERSION;
		vers->version_major = VERSION_MAJOR;
		vers->version_minor = VERSION_MINOR;
		software_timer_stop (&bl.startup);
		break;

	case BOOTLOADER_CMD_SENDPROMPT:
		// Пришли данные о файле
		bl.firmware.offset = 0;
		bl.firmware.crc32 = fprompt->crc32;
		bl.firmware.totalLength = fprompt->length;

		software_timer_stop (&bl.startup);

		// Вычисляем число стираемых страниц памяти. И вообще - влезет ли в нашу микросхемку столько
		if ((BOOTLOADER_PAGE_SIZE * (BOOTLOADER_TOTAL_PAGES_NUMBER - BOOTLOADER_APPLICATION_PAGE_NUMBER)) < bl.firmware.totalLength)
		{
			// Размер файла прошивки больше, чем есть памяти
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDPROMPT;
			ans->data[0] = BOOTLOADER_CONFIRM_ERROR;
		}
		else
		{
			// Снимаем блокировку записи flash
			FLASH_Unlock();

			// Высчитываем - сколько страниц надо затереть
			pagesNum = bl.firmware.totalLength / BOOTLOADER_PAGE_SIZE;
			if (bl.firmware.totalLength % BOOTLOADER_PAGE_SIZE) pagesNum++;

			// Затираем страницы памяти
			for (uint8_t k = 0; k < pagesNum; k++)
			{
				FLASH_ErasePage (BOOTLOADER_APPLICATION_ADDRESS + (k * BOOTLOADER_PAGE_SIZE));
				while (FLASH->SR & FLASH_SR_BSY);
			}

			// Отправляем ответ о готовности
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDPROMPT;
			ans->data[0] = conf;
		}
		break;

	case BOOTLOADER_CMD_SENDDATA:
		// Пришел кусок файла
		// Просчитаем CRC на этот кусочек. Если он первый - то сбрасываем аккумулятор CRC
		if (fdata->offset == bl.firmware.offset)
		{
			if (!fdata->offset) bl.firmware.crc32Calculated = crc32hw (fdata->data, fdata->len, 1);
			else bl.firmware.crc32Calculated = crc32hw (fdata->data, fdata->len, 0);

			if (fdata->offset == 0xfe0)
			{
				__nop();
			}

			// SAVE TO FLASH
			for (uint8_t k = 0; k < fdata->len; k += 4)
			{
				FLASH_ProgramWord (BOOTLOADER_APPLICATION_ADDRESS + fdata->offset + k, *(uint32_t*)(fdata->data + k));
				while (FLASH->SR & FLASH_SR_BSY);
			}
			bl.firmware.prevOffset = bl.firmware.offset;
			bl.firmware.offset += fdata->len;
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDDATA;
			ans->data[0] = conf;
		}
		else if (fdata->offset == bl.firmware.prevOffset)
		{
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDDATA;
			ans->data[0] = conf;
		}
		else
		{
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDDATA;
			ans->data[0] = BOOTLOADER_CONFIRM_ERROR;
		}
		break;

	case BOOTLOADER_CMD_SENDCHKSUM:
		// Пришла команда о проверке контрольных сумм и перезагрузке устройства
		if (bl.firmware.crc32Calculated == bl.firmware.crc32)
		{
			// Все ок. Прыгаем в новую аппликуху
			FLASH_Lock();
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDCHKSUM;
			ans->data[0] = conf;
			software_timer_start (&bl.startup, 500);
		}
		else
		{
			// Ошибка контрольных сумм. возвращаем сообщение о ошибке
			ans->len = 1;
			ans->nlen = 0xfe;
			ans->cmd = BOOTLOADER_CMD_SENDCHKSUM;
			ans->data[0] = BOOTLOADER_CONFIRM_ERROR;
		}
		break;

	default:
#ifdef BOOTLOADER_MODULE_UART
		resState ();
#endif /* BOOTLOADER_MODULE_UART */
		return 0;
	}

	crc = ans->data + ans->len;
	*crc = crc8 (&ans->cmd, ans->len + 3);

	// Сбросим флаг готовности
#ifdef BOOTLOADER_MODULE_UART
	resState ();
	return sizeof(bootloader_msg_t) + ans->len + 1;
#endif /* BOOTLOADER_MODULE_UART */

#ifdef BOOTLOADER_MODULE_SX1276
	return sizeof(bootloader_msg_t) + ans->len + 1;
#endif /* BOOTLOADER_MODULE_SX1276 */
}

static void runApplication (void)
{
	pFunction Jump_To_Application;
	__disable_irq();

	USART_DeInit (USART3);

	Jump_To_Application = (pFunction)(*(uint32_t*)(BOOTLOADER_APPLICATION_ADDRESS + 4));
	__set_MSP(*(uint32_t*)BOOTLOADER_APPLICATION_ADDRESS);
	Jump_To_Application();
}

#ifdef BOOTLOADER_MODULE_SX1276
static void radioTask (void)
{
	sx1276_LoRa_updateState (&T);

	if (!(T.Control.busy))
	{
#ifdef BOOTLOADER_RF_TRACT_CONTROL
		switchToRx ();
#endif /* BOOTLOADER_RF_TRACT_CONTROL */
	}

	if (T.receiver.readyRead)
	{
		answerLen = 0;
		T.receiver.readyRead = 0;
		
		if (memcmp (T.receiver.rxBuffer, prefix, sizeof (prefix)) == 0)
			answerLen = processPacket (T.receiver.rxBuffer, answer, BOOTLOADER_CONFIRM_SX1276);

		if (answerLen)
		{
#ifdef BOOTLOADER_RF_TRACT_CONTROL			
			switchToTx ();
#endif /* BOOTLOADER_RF_TRACT_CONTROL */			
			delay_ms (10);
			sx1276_LoRa_sendPacket (&T, answer, answerLen);
		}
	}
}
#endif /* BOOTLOADER_MODULE_SX1276 */
