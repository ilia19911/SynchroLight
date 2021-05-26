#include "include.h"

static uint8_t spiXmit (SPI_TypeDef *SPIx, uint8_t byte);
static void spiWriteReg (LPS331AP_Interface_t *SPI_Int, uint8_t addr, uint8_t byte);
static void spiWriteRegs (LPS331AP_Interface_t *SPI_Int, uint8_t addr, uint8_t *buffer, uint8_t length);
static uint8_t spiReadReg (LPS331AP_Interface_t *SPI_Int, uint8_t addr);
static void spiReadRegs (LPS331AP_Interface_t *SPI_Int, uint8_t addr, uint8_t *buffer, uint8_t length);

static void quadratureCompensationCalibrate (lps331ap_t *sense);
static void quadratureCompensation_Step1 (lps331ap_calibration_t *qCoef);
static void quadratureCompensation_Step2 (lps331ap_calibration_t *qCoef);
static void quadratureCompensation_Step3 (lps331ap_calibration_t *qCoef);
static double quadratureCompensation_Step4 (lps331ap_t *sense, double Pout, double Tout);

/* ========================================================================== */
/* Функция инициализации датчика давления/температуры													*/
/* ========================================================================== */
void lps331apInit (lps331ap_Init_Struct_t *init, lps331ap_t *sens)
{
	uint8_t temp = 0;
	
	// Подготовка интерфейса
	sens->interface.SPIx = init->SPIx;
	sens->interface.CS_PORT = init->CS_PORT;
	sens->interface.CS_PIN = init->CS_PIN;
	
	// Ждем, пока сенсор не ответит, что он это он
	while (temp < 10)
	{
		if ((spiReadReg (&sens->interface, LPS331AP_WHO_AM_I_REG)) == LPS331AP_WHO_AM_I_REG_VALUE) break;
		temp++;
	}
	if (temp == 10)
	{
		// Какой нибудь обработчик ошибки и вываливаемся из процедуры инициализации. Нет у нас значит датчика
		return;		
	}

	// Первым делом сбросим все внутренности сенсора
	spiWriteReg (&sens->interface, LPS331AP_CTRL2_REG, (1 << LPS331AP_CTRL2_REG_SWRESET_OFFSET));
	delay_ms (10);	
	spiWriteReg (&sens->interface, LPS331AP_CTRL2_REG, 0);
	
	// Настраиваем конфигурацию.
	// Первым делом настроим регистр усреднения
	temp = (init->pressure_averager | (init->temperature_averager << LPS331AP_RES_REG_AVGT_OFFSET));
	spiWriteReg (&sens->interface, LPS331AP_RES_REG, temp);
	
	// Регистр управления 1
	temp = (1 << LPS331AP_CTRL_REG_POWERDOWN_OFFSET) | (init->rate << LPS331AP_CTRL_REG_ODR_OFFSET);
	if (init->block_update) temp |= (1 << LPS331AP_CTRL_REG_BLOCKUPDATE_OFFSET);
	if (init->delta_pressure) temp |= (1 << LPS331AP_CTRL_REG_DELTAENABLE_OFFSET);
	if (init->spi_halfduplex) temp |= (1 << LPS331AP_CTRL_REG_SPIMODE_OFFSET);
	spiWriteReg (&sens->interface, LPS331AP_CTRL1_REG, temp);
	
	// Во втором регистре нам ничего не надо, поэтому переходим к третьему
	temp = (init->int2_src << LPS331AP_CTRL3_REG_INTSRC2_OFFSET) | (init->int1_src << LPS331AP_CTRL3_REG_INTSRC1_OFFSET);
	spiWriteReg (&sens->interface, LPS331AP_CTRL3_REG, temp);
	
	// Ну и выполним инициализацию квадратурной компенсации температуры, если это надо
	if (init->quadratureCompensation == LPS331AP_ENABLE)
	{
		sens->quadratureCompensator.enable = 1;
		quadratureCompensationCalibrate (sens);
	}
	else sens->quadratureCompensator.enable = 0;
}
/* ========================================================================== */

/* ========================================================================== */
/* Основная функция обработки данных. Считывает регистры с сенсора и 					*/
/* преобразует их в понятные юзеру																						*/
/* ========================================================================== */
void lps331apTask (lps331ap_t *sens)
{
	int32_t tempP;
	int16_t tempT;
	uint8_t buff[8];
	
	spiReadRegs (&sens->interface, LPS331AP_POUT_XL_REG, buff, 5);
	
	// Преобразуем данные в понятные
	tempP = buff[0] | (buff[1] << 8) | (buff[2] << 16);
	if (buff[2] & 0x80) tempP |= 0xff000000;
	sens->pressure.pressure = (float)tempP / 4096;
	
	tempT = buff[3] | (buff[4] << 8);
	sens->temperature = 42.5 + (float)tempT / 480;
	
	if (sens->eventType != NONE)
	{
		// Считаем регистр, в котором можно отследить, что произошло событие (превышение или понижение порога)
		buff[5] = spiReadReg (&sens->interface, LPS331AP_INTSRC_REG);
		if (sens->eventType == DIFF_PRESSURE_HIGHER)
		{
			if (buff[5]) sens->eventReady = 1;
		}
		else 
		{
			if (!(buff[5])) sens->eventReady = 1;
		}
	}
	
	// Расчет квадратурно компенсированного давления
	if (sens->quadratureCompensator.enable)
		sens->pressure.pressure_compensated = (float)quadratureCompensation_Step4 (sens, (double)tempP, (double)tempT) / 4096;
}
/* ========================================================================== */

/* ========================================================================== */
/* Функция для включения генерации события при превышении (понижении) давления*/
/* заданного порога. При проишествии события в структуре модуля взвешивается  */
/* флаг, который надо будет сбросить вручную. Также (при соответствующих)			*/
/* настройках, происходит генерация аппаратного прерывания датчиком						*/
/* Входные параметры: lps331ap_t *sense - указатель на экземпляр модуля				*/
/* 		float thsPressure - сам порог давления в мБар														*/
/* 		lps331ap_threshold_type_t type - тип порога															*/
/*			NONE - отключить генерацию события																		*/
/*			DIFF_PRESSURE_LOWER - срабатывание при снижении давления ниже порога	*/
/*			DIFF_PRESSURE_HIGHER - срабатывание при превышении давлением порога		*/
/* ========================================================================== */
void lps331ap_setPressureThreshold (lps331ap_t *sense, float thsPressure, lps331ap_threshold_type_t type)
{
	uint16_t t16;
	uint8_t t8;
		
	// Запишем значение порога в регистр
	t16 = (uint16_t)thsPressure * 16;
	sense->pressure.pressure_threshold = thsPressure;
	sense->eventType = type;
	spiWriteRegs (&sense->interface, LPS331AP_THS_P_LOW_REG, (uint8_t*)&t16, sizeof (uint16_t));
	
	// Теперь в зависимости от режима - включим тот, или иной бит в настройках эвент-генератора
	switch (type)
	{
		case NONE:
			t8 = 0;
			break;
		
		case DIFF_PRESSURE_HIGHER:
			t8 = (1 << LPS331AP_INT_CFG_PH_E);
			break;
		
		case DIFF_PRESSURE_LOWER:
			t8 = (1 << LPS331AP_INT_CFG_PL_E);
			break;
	}
	spiWriteReg (&sense->interface, LPS331AP_INTCFG_REG, t8);	
}
/* ========================================================================== */

/* ========================================================================== */
/* Задать опорный уровень давления																						*/
/* Например: в данный момент давление (абсолютное) составляет 1100 мБар				*/
/* если задать опорный уровень в 500 мБар, измеряемое датчиком давление				*/
/* составит 600 мБар.																													*/
/* Необходимо отметить, что при задании порога, нужно учитывать величину			*/
/* опорного давления. Сравнивается порог и резальутат - абсолютное - опорное	*/
/* ========================================================================== */
void lps331ap_setReferencePressure (lps331ap_t *sense, float pressure)
{
	uint32_t t32;
	
	t32 = (uint32_t)pressure * 4096;
	sense->pressure.pressure_reference = pressure;
	
	spiWriteRegs (&sense->interface, LPS331AP_REF_XL_REG, (uint8_t*)&t32, sizeof (uint32_t) - 1);		// Надо отправить
}
/* ========================================================================== */

/* ========================================================================== */
/* Функция разрешения работы генератора события по порогу											*/
/* ========================================================================== */
void lps331ap_diffCircuitCmd (lps331ap_t *sense, lps331ap_state_t funcState)
{
	uint8_t st;
	
	st = spiReadReg (&sense->interface, LPS331AP_CTRL1_REG);
	
	if (funcState) st |= (1 << LPS331AP_CTRL_REG_DIFFEN_OFFSET);
	else st &=~ (1 << LPS331AP_CTRL_REG_DIFFEN_OFFSET);
	
	spiWriteReg (&sense->interface, LPS331AP_CTRL1_REG, st);
}
/* ========================================================================== */

/* ========================================================================== */
/* статик функции, для работы с SPI интерфейсом																*/
/* ========================================================================== */
/* приемо передача по SPI																											*/
/* ========================================================================== */
static uint8_t spiXmit (SPI_TypeDef *SPIx, uint8_t byte)
{
	SPIx->DR = byte;
	while (!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}
/* ========================================================================== */

/* ========================================================================== */
/* Запись регистра																														*/
/* ========================================================================== */
static void spiWriteReg (LPS331AP_Interface_t *SPI_Int, uint8_t addr, uint8_t byte)
{
	SPI_Int->CS_PORT->ODR &=~ SPI_Int->CS_PIN;
	spiXmit (SPI_Int->SPIx, addr);
	spiXmit (SPI_Int->SPIx, byte);
	SPI_Int->CS_PORT->ODR |= SPI_Int->CS_PIN;
}
/* ========================================================================== */

/* ========================================================================== */
/* Чтение регистра																														*/
/* ========================================================================== */
static uint8_t spiReadReg (LPS331AP_Interface_t *SPI_Int, uint8_t addr)
{
	uint8_t tmp;
	
	SPI_Int->CS_PORT->ODR &=~ SPI_Int->CS_PIN;
	spiXmit (SPI_Int->SPIx, addr | LPS331AP_READ_SIGN);
	tmp = spiXmit (SPI_Int->SPIx, 0x55);
	SPI_Int->CS_PORT->ODR |= SPI_Int->CS_PIN;	
	return tmp;
}
/* ========================================================================== */

/* ========================================================================== */
/* Чтение последовательности регистров																				*/
/* ========================================================================== */
static void spiReadRegs (LPS331AP_Interface_t *SPI_Int, uint8_t addr, uint8_t *buffer, uint8_t length)
{
	uint8_t n;
	
	SPI_Int->CS_PORT->ODR &=~ SPI_Int->CS_PIN;
	spiXmit (SPI_Int->SPIx, addr | LPS331AP_READ_SIGN | LPS331AP_ADDRINC_SIGN);
	for (n = 0; n < length; n++)
		*(buffer + n) = spiXmit (SPI_Int->SPIx, 0x55);
	SPI_Int->CS_PORT->ODR |= SPI_Int->CS_PIN;	
}
/* ========================================================================== */

/* ========================================================================== */
/* Запись последовательности регистров																				*/
/* ========================================================================== */
static void spiWriteRegs (LPS331AP_Interface_t *SPI_Int, uint8_t addr, uint8_t *buffer, uint8_t length)
{
	uint8_t n;
	
	SPI_Int->CS_PORT->ODR &=~ SPI_Int->CS_PIN;
	spiXmit (SPI_Int->SPIx, addr | LPS331AP_ADDRINC_SIGN);
	for (n = 0; n < length; n++)
		spiXmit (SPI_Int->SPIx, *(buffer + n));
	SPI_Int->CS_PORT->ODR |= SPI_Int->CS_PIN;
}
/* ========================================================================== */

/* ========================================================================== */
/* Здесь начинается Ересь ST																									*/
/* Все используемые ниже магические числа, не менее магические адреса					*/
/* не указаны ни в одном даташите.																						*/
/* Откуда все это взято?																											*/
/* На сайте ST, в ветке частых вопросов по данному датчику звучал вопрос про	*/
/* про квадратурную компенсацию температуры. В основном даташите есть методика*/
/* краеугольным камнем которой являются некоторые коэффициенты, которые вшиты */
/* в каждый сенсор. Как ответ на вопрос про компенсацию ST ответили просто -  */
/* выложили экзампл с кодом.																									*/
/* в итоге ниже - код от ST, очищенный от привязки к Discovery								*/
/* ========================================================================== */
static void quadratureCompensationCalibrate (lps331ap_t *sense)
{
	lps331ap_calibration_t qCoeff;
	uint32_t temp;
	uint8_t regs[12];
	
	// Получим зашитые в кристалл коээфициенты для квадратурной компенсации
	spiReadRegs (&sense->interface, 0x30, regs, 11);
	
	qCoeff.TSL = regs[0] >> 2;
	qCoeff.TSL <<= 7;
	
	qCoeff.TSH = regs[1] & 0x3f;
	qCoeff.TSH <<= 7;
	
	temp = regs[1] >> (8-2);
	temp |= ((u16)regs[2]) << 2;
	temp |= ((u16)regs[3] & 0x03) << 10;
	qCoeff.TCV1 = temp;
	
	temp = regs[3] >> (8-6);
	temp |= ((u16)regs[4] & 0x3F) << 6;
	qCoeff.TCV2 = temp;
	
	temp = regs[4] >> (8-2);
	temp |= ((u16)regs[5]) << 2;
	temp |= ((u16)regs[6] & 0x03) << 10;
	qCoeff.TCV3 = temp;

	temp = regs[6] >> (8-6);
	temp |= ((u16)regs[7] & 0x0F) << 6;
	qCoeff.TCS1 = temp;

	temp = regs[7] >> (8-4);
	temp |= ((u16)regs[8] & 0x3F) << 4;
	qCoeff.TCS2 = temp;

	temp = regs[8] >> (8-2);
	temp |= ((u16)regs[9]) << 2;
	qCoeff.TCS3 = temp;

	spiReadRegs (&sense->interface, 0x18, regs, 3);
	temp = regs[0]>>(8-6);
	temp |= ((u16)regs[1]) << 6;
	temp |= ((u32)(regs[2]&0x0F))<< 14;
	qCoeff.DGain = temp;
	
	spiReadRegs (&sense->interface, 0x15, regs, 2);
	temp = regs[0]>>(8-6);
	temp |= ((u16)(regs[1]&0x1F))<< 6;
	temp <<= 5;
	qCoeff.DOffset = ((s16) temp) >> 5;
	
	quadratureCompensation_Step1 (&qCoeff);
	quadratureCompensation_Step2 (&qCoeff);
	quadratureCompensation_Step3 (&qCoeff);
	
	sense->quadratureCompensator.a = qCoeff.a;
	sense->quadratureCompensator.b = qCoeff.b;
	sense->quadratureCompensator.e1 = qCoeff.e1;
	sense->quadratureCompensator.e2 = qCoeff.e2;
}

#define T1												((double)-15600)
#define T2												((double)0)
#define T3												((double)13200)
#define P0												((double)1000)

static void quadratureCompensation_Step1 (lps331ap_calibration_t *qCoef)
{
	double t1, t2, t3;
	
	// По картинке, начинаем вычислять коэффициенты P2, и P1. P3 приравнен 0
	qCoef->P3 = 0;
	
	// Расчитываем P2
	t1 = (qCoef->TCV2 * pow (2, 16)) + qCoef->TCS2*128*(P0 - 760);
	t2 = (pow (2, 20) + pow (2, 5) * qCoef->DGain);
	t3 = t1/t2;
	t1 = (T2 - T3);
	t2 = t1 * t3;
	qCoef->P2 = qCoef->P3 + t2;
	
	// Расчитываем P1
	t1 = (qCoef->TCV1 * pow (2, 16)) + qCoef->TCS1*128*(P0 - 760);
	t2 = (pow (2, 20) + pow (2, 5) * qCoef->DGain);
	t3 = t1/t2;
	t1 = (T1 - T2);
	t2 = t1 * t3;
	qCoef->P1 = qCoef->P2 + t2;
}

static void quadratureCompensation_Step2 (lps331ap_calibration_t *qCoef)
{
	double Determinant;

	Determinant = pow(T1,2)*(T2 - T3) - pow(T2,2)*(T1-T3) + pow(T3,2)*(T1-T2);
	qCoef->a = (qCoef->P1/Determinant)*(T2-T3) - (qCoef->P2/Determinant)*(T1-T3) + (qCoef->P3/Determinant)*(T1-T2);
	qCoef->b =(-qCoef->P1/Determinant)*(pow(T2,2)-pow(T3,2)) + (qCoef->P2/Determinant)*(pow(T1,2)-pow(T3,2)) +(-qCoef->P3/Determinant)*(pow(T1,2)-pow(T2,2));
	qCoef->c = (qCoef->P1/Determinant)*(T3*pow(T2,2)-pow(T3,2)*T2) + (-qCoef->P2/Determinant)*(T3*pow(T1,2)-pow(T3,2)*(T1)) + (qCoef->P3/Determinant)*(T2*pow(T1,2)-pow(T2,2)*(T1));
}

static void quadratureCompensation_Step3 (lps331ap_calibration_t *qCoef)
{
	double Det2 = T2 - T3;
	qCoef->e2 = (qCoef->P2/Det2)*(1) + (-qCoef->P3/Det2)*(1);
	qCoef->d2 = (-1)*(qCoef->P2/Det2)*(T3) + (qCoef->P3/Det2)*(T2);
	qCoef->e1 = (qCoef->P1 - qCoef->d2)/T1;
	qCoef->d1 = qCoef->d2;
}

static double quadratureCompensation_Step4 (lps331ap_t *sense, double Pout, double Tout)
{
	double PComp, t1, t2;
	
	t1 = sense->quadratureCompensator.a * Tout * Tout;
	if (Tout < 0)	t2 = (sense->quadratureCompensator.b - sense->quadratureCompensator.e1) * Tout;
	else t2 = (sense->quadratureCompensator.b - sense->quadratureCompensator.e2) * Tout;
	
	PComp = Pout + t1 + t2;
	
	return PComp;
}
