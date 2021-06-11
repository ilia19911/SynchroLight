#include "include.h"

// Модуль протокола UBX для получения координат и прочего, прочего и прочего
// За авторством Харитоненко В.В.
static ubx_ChkSumm_t ubxCrcCalc (uint8_t *data, uint16_t length);
static void ubxProcessByte (ubx_t *ubx, uint8_t ch);
static void inline ubxResetState (ubx_t *ubx);
static void inline ubxStateUp (ubx_t *ubx);
static void ubxProcessPacket (ubx_t *ubx);

#ifdef UBX_TEST
/* TEST SECTION */
uint8_t test_num = 0;
uint8_t msg1[] = {0xB5, 0x62, 0x01, 0x03, 0x10, 0x00, 0xDE, 0x4B, 0x87, 0x17, 0x03, 0xDD, 0x00, 0x00, 0x2E, 0x78, 0x00, 0x00, 0x5A, 0x10, 0x02, 0x00, 0xCD, 0x36};
uint8_t msg2[] = {0xB5, 0x62, 0x01, 0x02, 0x1C, 0x00, 0xDE, 0x4B, 0x87, 0x17, 0x8B, 0x37, 0x0C, 0x12, 0x03, 0x7C, 0xBD, 0x23, 0xEC, 0x40, 0x00, 0x00, 0xCF, 0x02, 0x00, 0x00, 0x80, 0x0C, 0x00, 0x00, 0xA1, 0x11, 0x00, 0x00, 0x60, 0xA9};
uint8_t msg3[] = {0xB5, 0x62, 0x01, 0x12, 0x24, 0x00, 0xDE, 0x4B, 0x87, 0x17, 0x30, 0x02, 0x00, 0x00, 0x85, 0xFD, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x4F, 0x03, 0x00, 0x00, 0x4F, 0x03, 0x00, 0x00, 0xDB, 0x30, 0xDB, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x7C, 0x0D, 0x06, 0x00, 0x04, 0x32};
uint8_t msg4[] = {0xB5, 0x62, 0x01, 0x21, 0x14, 0x00, 0xDE, 0x4B, 0x87, 0x17, 0xF3, 0x03, 0x00, 0x00, 0xA3, 0x67, 0xB1, 0x2C, 0xE1, 0x07, 0x02, 0x09, 0x0D, 0x26, 0x2C, 0xF7, 0x23, 0x00};
uint8_t msg5[] = {0xB5, 0x62, 0x01, 0x06, 0x34, 0x00, 0xD8, 0x4C, 0x87, 0x17, 0x76, 0x50, 0xFD, 0xFF, 0x8F, 0x07, 0x03, 0xDD, 0x82, 0xB7, 0x79, 0x10, 0x5E, 0x90, 0x9E, 0x09, 0xBB, 0xDB, 0xC5, 0x20, 0x2B, 0x02, 0x00, 0x00, 0x98, 0xFF, 0xFF, 0xFF, 0xE9, 0xFC, 0xFF, 0xFF, 0x08, 0x01, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x35, 0x01, 0x02, 0x07, 0x86, 0x4C, 0x02, 0x00, 0xF5, 0x55};
#endif
	
void ubxInit (ubx_t *ubx, uart_t *uart, uint32_t speed)
{
	uint8_t ubxBuffer[64];
	
	ubx_msg_t *msg = (void*)ubxBuffer;
	ubx_msg_portconfig_t *portCfg = (void*)msg->data;
	ubx_msg_msgconfig_t *msgCfg = (void*)msg->data;
	ubx_msg_rateconfig_t *rateCfg = (void*)msg->data;
	
	// Инициализация модуля в режиме ubx. Сначала настроим скорость на заданную, т.к. возможно, что
	// после включения модуль на скорости 9600
	uart_init (uart, 9600);
	
	uart_put_block (uart, "ABCDEF", 6);
	delay_ms (100);
	
	msg->ubxHeader1 = UBX_HEADER1;
	msg->ubxHeader2 = UBX_HEADER2;
	msg->messageClass = UBX_MSG_CLASS_CFG;
	msg->messageID = UBX_MSG_CFG_PRT;
	msg->messageLength = UBX_MSGLEN_CFG_PRT;
	
	portCfg->portID = UBX_PORT_USART1;
	portCfg->mode = UBX_PORT_MASK_STD;
	portCfg->speed = speed;
	portCfg->inProto = UBX_PORT_PROTO_MASK_UBX;
	portCfg->outProto = UBX_PORT_PROTO_MASK_UBX;
	portCfg->reserved1 = 0;
	portCfg->reserved2 = 0;
	portCfg->reserved3 = 0;
	portCfg->reserved4 = 0;
	portCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	
	// Выждем и переключимся на новую скорость
	delay_ms (100);
	uart_init (uart, speed);
	
	// Теперь скнофигуририуем нужные нам сообщения - NAV-TIMEUTC, NAV-NED, NAV-SVINFO, NAV-POSLLH, NAV-STATUS
	msg->messageID = UBX_MSG_CFG_MSG;
	msg->messageLength = UBX_MSGLEN_CFG_MSG;
	msgCfg->rate[0] = UBX_HW_I2C_OUTPUT;					// I2C
	msgCfg->rate[1] = UBX_HW_UART1_OUTPUT;				// UART1
	msgCfg->rate[2] = UBX_HW_UART2_OUTPUT;				// UART2
	msgCfg->rate[3] = UBX_HW_USB_OUTPUT;					// USB
	msgCfg->rate[4] = UBX_HW_SPI_OUTPUT;					// SPI
	msgCfg->rate[5] = 0;											// Reserved
	msgCfg->messageClass = UBX_MSG_CLASS_NAV;
	
	// Включаем отправку данных по решению
	msgCfg->messageID = UBX_MSG_NAV_SOL;
	msgCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	delay_ms (5);
		
	// Включаем отправку данных по времени
	msgCfg->messageID = UBX_MSG_NAV_TIMEUTC;
	msgCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	delay_ms (5);
	
	// Включаем отправку данных по скоростям
	msgCfg->messageID = UBX_MSG_NAV_VELNED;
	msgCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	delay_ms (5);
	
	// Включаем отправку данных по координатам в формате LLH (Lattitude, Longitude, Height)
	msgCfg->messageID = UBX_MSG_NAV_POSLLH;
	msgCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	delay_ms (5);

	// Включаем отправку данных о текущем состоянии решения	
	msgCfg->messageID = UBX_MSG_NAV_STATUS;
	msgCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	delay_ms (5);

	// И выставим интересующий нас рэйт. (в милисекундах)
	msg->messageID = UBX_MSG_CFG_RATE;
	msg->messageLength = UBX_MSGLEN_CFG_RATE;
	rateCfg->measureRate = 10000 / UBX_DATA_RATE;
	rateCfg->navRate = 1;
	rateCfg->timeRef = 1;
	rateCfg->ckSumm = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	uart_put_block (uart, ubxBuffer, msg->messageLength + UBX_SEND_HEADERS_SIZE);
	delay_ms (5);
	
	// Сохраним номер уарта в структуру
	ubx->receiver.uart = uart;
	ubx->receiver.pnt = ubx->receiver.buffer;
	ubx->receiver.state = 0;
}


// Функция подсчета контрольной суммы по стандарту протокола UBX
// Это функция именно расчета. Надо помнить, что под расчет по протоколу попадают все байты, кроме первых двух (хидер)
// Входные параметры: uint8_t *data - указатель на начало обсчета (поле Message Class)
//										uint16_t length - число байт в сообщении
// Возвращаемое значение: ubxChkSumm_t - два байта контрольной суммы. CkA и CkB
static ubx_ChkSumm_t ubxCrcCalc (uint8_t *data, uint16_t length)
{
	ubx_ChkSumm_t temp = {0, 0};
	
	for (int i = 0; i < length; i++)
	{
		temp.ckA = temp.ckA + *(data + i);
		temp.ckB = temp.ckB + temp.ckA;
	}
	
	return temp;
}

// Побайтно вычищаем fifo и обрабатываем тем самым все входящие пакеты
void ubxTask (ubx_t *ubx)
{
#ifdef 	UBX_TEST
	/* TEST SECTION! */
	switch (test_num)
	{
		case 0:
			memcpy (ubx->receiver.buffer, msg1, sizeof (msg1));
			test_num = 1;
			break;
		case 1:
			memcpy (ubx->receiver.buffer, msg2, sizeof (msg2));
			test_num = 2;
			break;
		case 2:
			memcpy (ubx->receiver.buffer, msg3, sizeof (msg3));
			test_num = 3;
			break;
		case 3:
			memcpy (ubx->receiver.buffer, msg4, sizeof (msg4));
			test_num = 4;
			break;
		case 4:
			memcpy (ubx->receiver.buffer, msg5, sizeof (msg5));
			test_num = 0;
			break;
		
		default:
			test_num = 0;
			break;
	}
	
	ubxProcessPacket (ubx);
#else
	int n;
	
	while (1)
	{
		n = uart_getc (ubx->receiver.uart);
		if (n >= 0) ubxProcessByte (ubx, (uint8_t)n);
		else break;
	}
#endif	
}

void ubxProcessByte (ubx_t *ubx, uint8_t ch)
{
	*ubx->receiver.pnt = ch;
	
	switch (ubx->receiver.state)
	{
		case 0:
			// Ждем первый старт байт
			if (ch == UBX_HEADER1) ubxStateUp (ubx);
			break;
		
		case 1:
			// Второй старт байт
			if (ch == UBX_HEADER2) ubxStateUp (ubx);
			else ubxResetState (ubx);
			break;
		
		case 2:
			// Класс сообщения. Интересует только NAV сообщения
			if (ch == UBX_MSG_CLASS_NAV) ubxStateUp (ubx);
			else ubxResetState (ubx);
			break;
		
		case 3:
			// Тип сообщения. Примем все
			ubxStateUp (ubx);
			break;
		
		case 4:
			// Младшая часть длины посылки
			ubx->receiver.length = ch;
			ubxStateUp (ubx);
			break;
		
		case 5:
			// Старшая часть посылки
			ubx->receiver.length |= (ch << 8);
			ubxStateUp (ubx);
			break;
		
		case 6:
			// Принимаем тело сообщения
			ubx->receiver.length--;
			if (!(ubx->receiver.length)) ubxStateUp (ubx);
			else ubx->receiver.pnt++;
			break;
		
		case 7:
			// Первый байт контрольнной суммы
			ubxStateUp (ubx);
			break;
		
		case 8:
			// Второй байт контрольной суммы. Сообщение принято
			ubxProcessPacket (ubx);
			ubxResetState (ubx);
			break;
		
		default: 
			// Тут мы вообще оказаться не должны, но если вдруг - сбросим автомат
			ubxResetState (ubx);
			break;
	}
}

static void inline ubxResetState (ubx_t *ubx)
{
	ubx->receiver.pnt = ubx->receiver.buffer;
	ubx->receiver.state = 0;
}
	
static void inline ubxStateUp (ubx_t *ubx)
{
	ubx->receiver.pnt++;
	ubx->receiver.state++;
}

static void ubxProcessPacket (ubx_t *ubx)
{
	ubx_msg_t *msg = (void*)ubx->receiver.buffer;
	ubx_ChkSumm_t ckSum;
	
	struct tm curTime;
		
	// Для начала проверим CRC
	ckSum = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
	if ((ckSum.ckA != *(uint8_t*)(&msg->messageClass + msg->messageLength + UBX_CRCCALC_HEADERS_SIZE)) || (ckSum.ckB != *(uint8_t*)(&msg->messageClass + msg->messageLength + UBX_CRCCALC_HEADERS_SIZE + 1))) return;
	
	// CRC сошлось. Теперь распарсим данные
	switch (msg->messageID)
	{
		case UBX_MSG_NAV_SOL:
			memcpy ((void*)&ubx->rawData.sol, msg->data, sizeof (ubx_nav_sol_t));
			// Тут возьмем только количество спутников и тип фиксации
			ubx->aggregatedData.fixMode = ubx->rawData.sol.gpsFixType;
			ubx->aggregatedData.numSv = ubx->rawData.sol.numSV;
			break;
		
		case UBX_MSG_NAV_POSLLH:
			memcpy ((void*)&ubx->rawData.posllh, msg->data, sizeof (ubx_nav_posllh_t));
			// Здесь нам нужны координаты
			ubx->aggregatedData.lattitude = (double)ubx->rawData.posllh.lattitude / 10000000;
			ubx->aggregatedData.longitude = (double)ubx->rawData.posllh.longitude / 10000000;
			ubx->aggregatedData.height = (double)ubx->rawData.posllh.heightAboveSeaLevel / 1000;
			break;
		
		case UBX_MSG_NAV_VELNED:
			memcpy ((void*)&ubx->rawData.ned, msg->data, sizeof (ubx_nav_velned_t));
			// Скорость
			ubx->aggregatedData.speed2d = (float)ubx->rawData.ned.speed2d / 100;
			ubx->aggregatedData.speed3d = (float)ubx->rawData.ned.speed3d / 100;
			ubx->aggregatedData.azimuth = (float)ubx->rawData.ned.heading / 100000;
			break;
		
		case UBX_MSG_NAV_TIMEUTC:
			memcpy ((void*)&ubx->rawData.timeutc, msg->data, sizeof (ubx_nav_timeutc_t));
			// Время
			ubx->aggregatedData.year = ubx->rawData.timeutc.year;
			ubx->aggregatedData.month = ubx->rawData.timeutc.month;
			ubx->aggregatedData.day = ubx->rawData.timeutc.day;
			ubx->aggregatedData.hour = ubx->rawData.timeutc.hour;
			ubx->aggregatedData.min = ubx->rawData.timeutc.min;
			ubx->aggregatedData.sec = ubx->rawData.timeutc.sec;
			
			curTime.tm_year = ubx->rawData.timeutc.year - 1900;
			curTime.tm_mon = ubx->rawData.timeutc.month - 1;
			curTime.tm_mday = ubx->rawData.timeutc.day;
			curTime.tm_hour = ubx->rawData.timeutc.hour;
			curTime.tm_min = ubx->rawData.timeutc.min;
			curTime.tm_sec = ubx->rawData.timeutc.sec;
			ubx->aggregatedData.timestamp = mktime (&curTime);
			break;

		default: break;
	}
}
