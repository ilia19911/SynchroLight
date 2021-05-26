#include <stdio.h>
#include "stm32f10x_conf.h"
#include "integer.h"

#include "include.h"

/*
 * Code is split into 3 parts:
 * - generic SPI code: adapt for your MCU
 * - sd card code, with crc7 and crc16 calculations
 *   there's lots of it, but it's simple
 * - fatfs interface. If you use anything else, look here for
 *   interface to SD card code
 */

struct hwif {
	int initialized;
	int sectors;
	int erase_sectors;
	int capabilities;
};
typedef struct hwif hwif;

#define CAP_VER2_00	(1<<0)
#define CAP_SDHC	(1<<1)


enum sd_speed { SD_SPEED_INVALID, SD_SPEED_400KHZ, SD_SPEED_25MHZ };

#define spi_cs_low()							pin_clr(PIN_SPI_FLASH_SD_CS)
#define spi_cs_high()							pin_set(PIN_SPI_FLASH_SD_CS)

static void spi_set_speed(enum sd_speed speed)
{
}

static u8 spi_txrx(u8 data)
{
	/* RXNE always happens after TXE, so if this function is used
	 * we don't need to check for TXE */
	uint8_t byte;
	
	byte = spi_transfer (data);
	return byte;
}

/* crc helpers */
static u8 crc7_one(u8 t, u8 data)
{
	int i;
	const u8 g = 0x89;

	t ^= data;
	for (i=0; i<8; i++) {
		if (t & 0x80)
			t ^= g;
		t <<= 1;
	}
	return t;
}

u8 crc7(const u8 *p, int len)
{
	int j;
	u8 crc = 0;
	for (j=0; j<len; j++)
		crc = crc7_one(crc, p[j]);

	return crc>>1;
}

/* http://www.eagleairaust.com.au/code/crc16.htm */
static u16 crc16_ccitt(u16 crc, u8 ser_data)
{
	crc  = (u8)(crc >> 8) | (crc << 8);
	crc ^= ser_data;
	crc ^= (u8)(crc & 0xff) >> 4;
	crc ^= (crc << 8) << 4;
	crc ^= ((crc & 0xff) << 4) << 1;

	return crc;
}
u16 crc16(const u8 *p, int len)
{
	int i;
	u16 crc = 0;

	for (i=0; i<len; i++)
		crc = crc16_ccitt(crc, p[i]);

	return crc;
}


/*** sd functions - on top of spi code ***/

static void sd_cmd(u8 cmd, u32 arg)
{
	u8 crc = 0;
	spi_txrx(0x40 | cmd);
	crc = crc7_one(crc, 0x40 | cmd);
	spi_txrx(arg >> 24);
	crc = crc7_one(crc, arg >> 24);
	spi_txrx(arg >> 16);
	crc = crc7_one(crc, arg >> 16);
	spi_txrx(arg >> 8);
	crc = crc7_one(crc, arg >> 8);
	spi_txrx(arg);
	crc = crc7_one(crc, arg);
	//spi_txrx(0x95);	/* crc7, for cmd0 */
	spi_txrx(crc | 0x1);	/* crc7, for cmd0 */
}

static u8 sd_get_r1()
{
	int tries = 1000;
	u8 r;

	while (tries--) {
		r = spi_txrx(0xff);
		if ((r & 0x80) == 0)
			return r;
	}
	return 0xff;
}

static u16 sd_get_r2()
{
	int tries = 1000;
	u16 r;

	while (tries--) {
		r = spi_txrx(0xff);
		if ((r & 0x80) == 0)
			break;
	}
	if (tries < 0)
		return 0xff;
	r = r<<8 | spi_txrx(0xff);

	return r;
}

/*
 * r1, then 32-bit reply... same format as r3
 */
static u8 sd_get_r7(u32 *r7)
{
	u32 r;
	r = sd_get_r1();
	if (r != 0x01)
		return r;

	r = spi_txrx(0xff) << 24;
	r |= spi_txrx(0xff) << 16;
	r |= spi_txrx(0xff) << 8;
	r |= spi_txrx(0xff);

	*r7 = r;
	return 0x01;
}
#define sd_get_r3 sd_get_r7

//static const char *r1_strings[7] = {
//	"in idle",
//	"erase reset",
//	"illegal command",
//	"communication crc error",
//	"erase sequence error",
//	"address error",
//	"parameter error"
//};

//static void print_r1(u8 r)
//{
//	int i;
//	dprintf("R1: %02x\n", r);
//	for (i=0; i<7; i++)
//		if (r & (1<<i))
//			dprintf("  %s\n", r1_strings[i]);
//}

//static const char *r2_strings[15] = {
//	"card is locked",
//	"wp erase skip | lock/unlock cmd failed",
//	"error",
//	"CC error",
//	"card ecc failed",
//	"wp violation",
//	"erase param",
//	"out of range | csd overwrite",
//	"in idle state",
//	"erase reset",
//	"illegal command",
//	"com crc error",
//	"erase sequence error",
//	"address error",
//	"parameter error",
//};

//static void print_r2(u16 r)
//{
//	int i;
//	dprintf("R2: %04x\n", r);
//	for (i=0; i<15; i++)
//		if (r & (1<<i))
//			dprintf("  %s\n", r2_strings[i]);
//}

/* Nec (=Ncr? which is limited to [0,8]) dummy bytes before lowering CS,
 * as described in sandisk doc, 5.4. */
static void sd_nec()
{
	int i;
	for (i=0; i<8; i++)
		spi_txrx(0xff);
}


static int sd_init(hwif *hw)
{
	int i;
	int r;
	u32 r7;
	u32 r3;
	int tries;

	hw->capabilities = 0;

	/* start with 100-400 kHz clock */
	spi_set_speed(SD_SPEED_400KHZ);

	spi_cs_high();
	/* 74+ clocks with CS high */
	for (i=0; i<10; i++)
		spi_txrx(0xff);

	/* reset */
	spi_cs_low();
	sd_cmd(0, 0);
	r = sd_get_r1();
	sd_nec();
	spi_cs_high();
	if (r == 0xff)
		return -1;
	if (r != 0x01) {
		return -2;
	}

	/* ask about voltage supply */
	spi_cs_low();
	sd_cmd(8, 0x1aa /* VHS = 1 */);
	r = sd_get_r7(&r7);
	sd_nec();
	spi_cs_high();
	hw->capabilities |= CAP_VER2_00;
	if (r == 0xff)
		return -1;
	if (r == 0x01)
		{
		}
	else if (r & 0x4) {
		hw->capabilities &= ~CAP_VER2_00;
	} else {
		return -2;
	}
	/* ask about voltage supply */
	spi_cs_low();
	sd_cmd(58, 0);
	r = sd_get_r3(&r3);
	sd_nec();
	spi_cs_high();
	if (r == 0xff)
		return -1;
	if (r != 0x01 && !(r & 0x4)) { /* allow it to not be implemented - old cards */
		return -2;
	}
	else {
		int i;
		for (i=4; i<=23; i++)
			if (r3 & 1<<i)
				break;
		for (i=23; i>=4; i--)
			if (r3 & 1<<i)
				break;
		/* CCS shouldn't be valid here yet */
	}


	tries = 1000;
	u32 hcs = 0;
	/* say we support SDHC */
	if (hw->capabilities & CAP_VER2_00)
		hcs = 1<<30;

	/* needs to be polled until in_idle_state becomes 0 */
	do {
		/* send we don't support SDHC */
		spi_cs_low();
		/* next cmd is ACMD */
		sd_cmd(55, 0);
		r = sd_get_r1();
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			return -1;
		/* well... it's probably not idle here, but specs aren't clear */
		if (r & 0xfe) {
			return -2;
		}

		spi_cs_low();
		sd_cmd(41, hcs);
		r = sd_get_r1();
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			return -1;
		if (r & 0xfe) {
			return -2;
		}
	} while (r != 0 && tries--);
	if (tries == -1) {
		return -2;
	}

	/* Seems after this card is initialized which means bit 0 of R1
	 * will be cleared. Not too sure. */


	if (hw->capabilities & CAP_VER2_00) {
		/* ask about voltage supply */
		spi_cs_low();
		sd_cmd(58, 0);
		r = sd_get_r3(&r3);
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			return -1;
		if (r & 0xfe) {
			return -2;
		}
		else {
#if 1
			int i;
			for (i=4; i<=23; i++)
				if (r3 & 1<<i)
					break;
			for (i=23; i>=4; i--)
				if (r3 & 1<<i)
					break;
			/* CCS shouldn't be valid here yet */
			// XXX power up status should be 1 here, since we're finished initializing, but it's not. WHY?
			// that means CCS is invalid, so we'll set CAP_SDHC later
#endif
			if (r3>>30 & 1) {
				hw->capabilities |= CAP_SDHC;
			}

		}
	}


	/* with SDHC block length is fixed to 1024 */
	if ((hw->capabilities & CAP_SDHC) == 0) {
		spi_cs_low();
		sd_cmd(16, 512);
		r = sd_get_r1();
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			return -1;
		if (r & 0xfe) {
			return -2;
		}
	}

	/* crc on */
	spi_cs_low();
	sd_cmd(59, 0);
	r = sd_get_r1();
	sd_nec();
	spi_cs_high();
	if (r == 0xff)
		return -1;
	if (r & 0xfe) {
		return -2;
	}

	/* now we can up the clock to <= 25 MHz */
	spi_set_speed(SD_SPEED_25MHZ);

	return 0;
}

static int sd_read_status(hwif *hw)
{
	u16 r2;

	spi_cs_low();
	sd_cmd(13, 0);
	r2 = sd_get_r2();
	sd_nec();
	spi_cs_high();
	if (r2 & 0x8000)
		return -1;

	return 0;
}

/* 0xfe marks data start, then len bytes of data and crc16 */
static int sd_get_data(hwif *hw, u8 *buf, int len)
{
	int tries = 20000;
	u8 r;
	u16 _crc16;
	u16 calc_crc;
	int i;

	while (tries--) {
		r = spi_txrx(0xff);
		if (r == 0xfe)
			break;
	}
	if (tries < 0)
		return -1;

	for (i=0; i<len; i++)
		buf[i] = spi_txrx(0xff);

	_crc16 = spi_txrx(0xff) << 8;
	_crc16 |= spi_txrx(0xff);

	calc_crc = crc16(buf, len);
	if (_crc16 != calc_crc) {
		return -1;
	}

	return 0;
}

static int sd_put_data(hwif *hw, const u8 *buf, int len)
{
	u8 r;
	int tries = 10;
//	u8 b[16];
//	int bi = 0;
	u16 crc;

	spi_txrx(0xfe); /* data start */

	while (len--)
		spi_txrx(*buf++);

	crc = crc16(buf, len);
	/* crc16 */
	spi_txrx(crc>>8);
	spi_txrx(crc);

	/* normally just one dummy read in between... specs don't say how many */
	while (tries--) {
		r = spi_txrx(0xff);
		if (r != 0xff)
			break;
	}
	if (tries < 0)
		return -1;

	/* poll busy, about 300 reads for 256 MB card */
	tries = 100000;
	while (tries--) {
		if (spi_txrx(0xff) == 0xff)
			break;
	}
	if (tries < 0)
		return -2;

	/* data accepted, WIN */
	if ((r & 0x1f) == 0x05)
		return 0;

	return r;
}

static int sd_read_csd(hwif *hw)
{
	u8 buf[16];
	int r;
	int capacity;

	spi_cs_low();
	sd_cmd(9, 0);
	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		return -1;
	}
	if (r & 0xfe) {
		spi_cs_high();
		return -2;
	}

	r = sd_get_data(hw, buf, 16);
	sd_nec();
	spi_cs_high();
	if (r == -1) {
		return -3;
	}

	if ((buf[0] >> 6) + 1 == 1) {
	/* CSD v1 */
	capacity = (((buf[6]&0x3)<<10 | buf[7]<<2 | buf[8]>>6)+1) << (2+(((buf[9]&3) << 1) | buf[10]>>7)) << ((buf[5] & 0xf) - 9);
	} else {
	/* CSD v2 */
		/* this means the card is HC */
		hw->capabilities |= CAP_SDHC;

	capacity = buf[7]<<16 | buf[8]<<8 | buf[9]; /* in 512 kB */
	capacity *= 1024; /* in 512 B sectors */

	}

	hw->sectors = capacity;

	/* if erase_blk_en = 0, then only this many sectors can be erased at once
	 * this is NOT yet tested */
	hw->erase_sectors = 1;
	if (((buf[10]>>6)&1) == 0)
		hw->erase_sectors = ((buf[10]&0x3f)<<1 | buf[11]>>7) + 1;

	return 0;
}

static int sd_read_cid(hwif *hw)
{
	u8 buf[16];
	int r;

	spi_cs_low();
	sd_cmd(10, 0);
	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		return -1;
	}
	if (r & 0xfe) {
		spi_cs_high();
		return -2;
	}

	r = sd_get_data(hw, buf, 16);
	sd_nec();
	spi_cs_high();
	if (r == -1) {
		return -3;
	}

	return 0;
}


static int sd_readsector(hwif *hw, u32 address, u8 *buf)
{
	int r;

	spi_cs_low();
	if (hw->capabilities & CAP_SDHC)
		sd_cmd(17, address); /* read single block */
	else
		sd_cmd(17, address*512); /* read single block */

	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		r = -1;
		goto fail;
	}
	if (r & 0xfe) {
		spi_cs_high();
		r = -2;
		goto fail;
	}

	r = sd_get_data(hw, buf, 512);
	sd_nec();
	spi_cs_high();
	if (r == -1) {
		r = -3;
		goto fail;
	}

	return 0;
 fail:
	return r;
}

static int sd_writesector(hwif *hw, u32 address, const u8 *buf)
{
	int r;

	spi_cs_low();
	if (hw->capabilities & CAP_SDHC)
		sd_cmd(24, address); /* write block */
	else
		sd_cmd(24, address*512); /* write block */

	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		r = -1;
		goto fail;
	}
	if (r & 0xfe) {
		spi_cs_high();
		r = -2;
		goto fail;
	}

	spi_txrx(0xff); /* Nwr (>= 1) high bytes */
	r = sd_put_data(hw, buf, 512);
	sd_nec();
	spi_cs_high();
	if (r != 0) {
		r = -3;
		goto fail;
	}

	/* efsl code is weird shit, 0 is error in there?
	 * not that it's properly handled or anything,
	 * and the return type is char, fucking efsl */
	return 0;
 fail:
	return r;
}


/*** public API - on top of sd/spi code ***/

int hwif_init(hwif* hw)
{
	int tries = 10;

	if (hw->initialized)
		return 0;

	spi_init();

	while (tries--) {
		if (sd_init(hw) == 0)
			break;
	}
	if (tries == -1)
		return -1;

	/* read status register */
	sd_read_status(hw);

	sd_read_cid(hw);
	if (sd_read_csd(hw) != 0)
		return -1;

	hw->initialized = 1;
	return 0;
}

int sd_read(hwif* hw, u32 address, u8 *buf)
{
	int r;
	int tries = 10;

	r = sd_readsector(hw, address, buf);

	while (r < 0 && tries--) {
		if (sd_init(hw) != 0)
			continue;

		/* read status register */
		sd_read_status(hw);

		r = sd_readsector(hw, address, buf);
	}
	return r;
}

int sd_write(hwif* hw, u32 address,const u8 *buf)
{
	int r;
	int tries = 10;

	r = sd_writesector(hw, address, buf);

	while (r < 0 && tries--) {
		if (sd_init(hw) != 0)
			continue;

		/* read status register */
		sd_read_status(hw);

		r = sd_writesector(hw, address, buf);
	}

	return r;
}



/*** fatfs code that uses the public API ***/

#include "diskio.h"

hwif hw;

DSTATUS disk_initialize(BYTE drv)
{
	if (hwif_init(&hw) == 0)
		return 0;

	return STA_NOINIT;
}


DSTATUS disk_status(BYTE drv)
{
	if (hw.initialized)
		return 0;

	return STA_NOINIT;
}


DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
	int i;

	for (i=0; i<count; i++)
		if (sd_read(&hw, sector+i, buff+512*i) != 0)
			return RES_ERROR;

	return RES_OK;
}


#if _READONLY == 0
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
	int i;

	for (i=0; i<count; i++)
		if (sd_write(&hw, sector+i, buff+512*i) != 0)
			return RES_ERROR;

	return RES_OK;
}
#endif /* _READONLY */



DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff)
{
	switch (ctrl) {
	case CTRL_SYNC:
		return RES_OK;
	case GET_SECTOR_SIZE:
		*(WORD*)buff = 512;
		return RES_OK;
	case GET_SECTOR_COUNT:
		*(DWORD*)buff = hw.sectors;
		return RES_OK;
	case GET_BLOCK_SIZE:
		*(DWORD*)buff = hw.erase_sectors;
		return RES_OK;
	}
	return RES_PARERR;
}

// описание структуры времени FAT
typedef struct
{
	DWORD 	sec: 	5;	// биты 0-4, 1 ед. = 2 сек
	DWORD	min:	6;	// биты 5-10
	DWORD	hour:	5;	// биты 11-15
	DWORD	day:	5;	// биты 16-20
	DWORD	month:	4;	// биты 21-24
	DWORD	year:	7;	// биты 25-31
} fat_time_t;

DWORD get_fattime(void)
{
	union
	{
		fat_time_t fat_time;
		DWORD dword;
	} time;

	time.fat_time.sec = 0;
	time.fat_time.min = 0;
	time.fat_time.hour = 12;
	time.fat_time.day = 4;
	time.fat_time.month = 9;
	time.fat_time.year = 2013-1980;

	return time.dword;

}

WCHAR ff_convert (	/* Converted character, Returns zero on error */
	WCHAR	src,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
)
{
	return src;
}


WCHAR ff_wtoupper (	/* Upper converted character */
	WCHAR chr		/* Input character */
)
{
	return chr;
}
