#ifndef _LPS331AP_H_
#define _LPS331AP_H_

#include "math.h"

#define LPS331AP_READ_SIGN															((uint8_t)0x80)
#define LPS331AP_ADDRINC_SIGN														((uint8_t)0x40)

#define LPS331AP_WHO_AM_I_REG_VALUE											((uint8_t)0xbb)

#define LPS331AP_REF_XL_REG															((uint8_t)0x08)
#define LPS331AP_REF_L_REG															((uint8_t)0x09)
#define LPS331AP_REF_H_REG															((uint8_t)0x0a)
#define LPS331AP_WHO_AM_I_REG														((uint8_t)0x0f)
#define LPS331AP_RES_REG																((uint8_t)0x10)
#define LPS331AP_CTRL1_REG															((uint8_t)0x20)
#define LPS331AP_CTRL2_REG															((uint8_t)0x21)
#define LPS331AP_CTRL3_REG															((uint8_t)0x22)
#define LPS331AP_INTCFG_REG															((uint8_t)0x23)
#define LPS331AP_INTSRC_REG															((uint8_t)0x24)
#define LPS331AP_THS_P_LOW_REG													((uint8_t)0x25)
#define LPS331AP_THS_P_HI_REG														((uint8_t)0x26)
#define LPS331AP_STATUS_REG															((uint8_t)0x27)
#define LPS331AP_POUT_XL_REG														((uint8_t)0x28)
#define LPS331AP_POUT_L_REG															((uint8_t)0x29)
#define LPS331AP_POUT_H_REG															((uint8_t)0x2a)
#define LPS331AP_TOUT_L_REG															((uint8_t)0x2b)
#define LPS331AP_TOUT_H_REG															((uint8_t)0x2c)

#define LPS331AP_RES_REG_AVGP_OFFSET										0
#define LPS331AP_RES_REG_AVGT_OFFSET										4
#define LPS331AP_CTRL_REG_POWERDOWN_OFFSET							7
#define LPS331AP_CTRL_REG_ODR_OFFSET										4
#define LPS331AP_CTRL_REG_DIFFEN_OFFSET									3
#define LPS331AP_CTRL_REG_BLOCKUPDATE_OFFSET						2
#define LPS331AP_CTRL_REG_DELTAENABLE_OFFSET						1
#define LPS331AP_CTRL_REG_SPIMODE_OFFSET								0
#define LPS331AP_CTRL2_REG_SWRESET_OFFSET								2
#define LPS331AP_CTRL2_REG_AUTOZERO_OFFSET							1
#define LPS331AP_CTRL2_REG_ONESHOOT_OFFSET							0
#define LPS331AP_CTRL3_REG_INTSRC1_OFFSET								0
#define LPS331AP_CTRL3_REG_INTSRC2_OFFSET								3

#define LPS331AP_INT_CFG_PH_E														0
#define LPS331AP_INT_CFG_PL_E														1
#define LPS331AP_INT_CFG_LIR														2

typedef enum
{
	LPS331AP_Average_None,
	LPS331AP_Average_2_Samples,
	LPS331AP_Average_4_Samples,
	LPS331AP_Average_8_Samples,
	LPS331AP_Average_16_Samples,
	LPS331AP_Average_32_Samples,
	LPS331AP_Average_64_Samples,
	LPS331AP_Average_128_Samples,
	LPS331AP_Average_256_Samples,
	LPS331AP_Average_384_Samples,
	LPS331AP_Average_512_Samples
} lps331ap_averages_t;

typedef enum
{
	LPS331AP_RATE_OneShot,
	LPS331AP_RATE_1HzP_1HzT,
	LPS331AP_RATE_7HzP_1HzT,
	LPS331AP_RATE_12Hz5P_1HzT,
	LPS331AP_RATE_25HzP_1HzT,
	LPS331AP_RATE_7HzP_7HzT,
	LPS331AP_RATE_12Hz5P_12Hz5T,
	LPS331AP_RATE_25HzP_25HzT,
} lps331ap_outputrate_t;

typedef enum
{
	LPS331AP_DISABLE,
	LPS331AP_ENABLE
} lps331ap_state_t;

typedef enum
{
	LPS331AP_INTSRC_GND,
	LPS331AP_INTSRC_PressureHigh,
	LPS331AP_INTSRC_PressureLow,
	LPS331AP_INTSRC_PHigh_Or_PLow,
	LPS331AP_INTSRC_DataReady,
	LPS331AP_INTSRC_Reserved,
	LPS331AP_INTSRC_Reserved2,
	LPS331AP_INTSRC_TriState
} lps331ap_intsrc_t;

typedef struct
{
	SPI_TypeDef *SPIx;
	GPIO_TypeDef *CS_PORT;
	uint32_t CS_PIN;
	lps331ap_averages_t pressure_averager;
	lps331ap_averages_t temperature_averager;
	lps331ap_outputrate_t rate;
	lps331ap_state_t block_update;
	lps331ap_state_t delta_pressure;
	lps331ap_state_t spi_halfduplex;
	lps331ap_intsrc_t int1_src;
	lps331ap_intsrc_t int2_src;
	lps331ap_state_t quadratureCompensation;
} lps331ap_Init_Struct_t;

typedef struct 
{
	SPI_TypeDef *SPIx;
	GPIO_TypeDef *CS_PORT;
	uint32_t CS_PIN;
} LPS331AP_Interface_t;

typedef struct 
{
	float pressure;
	float pressure_compensated;
	float pressure_reference;
	float pressure_threshold;
} lps331ap_pressure_t;

typedef struct
{
	uint16_t TSL;
	uint16_t TSH;
	uint32_t TCV1;
	uint32_t TCV2;
	uint32_t TCV3;
	uint32_t TCS1;
	uint32_t TCS2;
	uint32_t TCS3;
	uint32_t DGain;
	int16_t DOffset;
	double TEMP1;
	double TEMP2;
	double TEMP3;	
	double P1;
	double P2;
	double P3;
	double a;
	double b;
	double c;	
	double determinant;
	double y1;
	double y2;
	double e1;
	double e2;
	double d1;
	double d2;
} lps331ap_calibration_t;

typedef struct 
{
	uint8_t enable;
	double a;
	double b;
	double e1;
	double e2;
} lps331ap_qComp_t;

typedef enum
{
	NONE,
	DIFF_PRESSURE_LOWER,
	DIFF_PRESSURE_HIGHER
} lps331ap_threshold_type_t;

typedef struct
{
	LPS331AP_Interface_t interface;
	lps331ap_pressure_t pressure;
	float temperature;
	uint8_t eventReady;
	lps331ap_threshold_type_t eventType;
	lps331ap_qComp_t quadratureCompensator;
} lps331ap_t;

void lps331apInit (lps331ap_Init_Struct_t *init, lps331ap_t *sens);
void lps331apTask (lps331ap_t *sens);
void lps331ap_setReferencePressure (lps331ap_t *sense, float pressure);
void lps331ap_setPressureThreshold (lps331ap_t *sense, float thsPressure, lps331ap_threshold_type_t type);
void lps331ap_diffCircuitCmd (lps331ap_t *sense, lps331ap_state_t funcState);

#endif /* _LPS331AP_H_ */
