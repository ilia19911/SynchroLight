#include "include.h"

static FATFS fs;
static char buff[256];

static FRESULT mounting (void);
static FRESULT scan_files (char *path);
static void sendFileToFlash (FIL *file, char *fname, uint8_t slot);

void sdTask (void)
{
	FRESULT res;	
	res = mounting ();
	
	if (res == FR_OK)
	{
		scan_files ("/"); // Сканируем корневой каталог со всем вложенным содержимым
	}
}

static FRESULT mounting (void)
{
	FRESULT result;
	
	dprintf ("\r\n========= SD Card inserted =========\r\n");

	result = f_mount (&fs, "", 0);
	if (result != FR_OK)
	{
		dprintf ("Mount failure\r\n");
		return FR_DISK_ERR;
	}
	else 
	{
		dprintf ("Mount complete\r\n");
		return FR_OK;
	}
}

static FRESULT scan_files (char *path)
{
	DIR dir;
	FILINFO fileInfo;
	FRESULT res;
	FIL file;
	int slotNum;
	
	res = f_opendir (&dir, path);
	if (res == FR_OK)														
	{
		for (;;)
		{
			res = f_readdir (&dir, &fileInfo);										// Считываем директорию
			if (res != FR_OK || fileInfo.fname[0] == 0) break;		// Если какая ошибка - отваливаемся
			dprintf ("File %s. File type: ", fileInfo.fname);
			if (strstr (fileInfo.fname, ".BIN"))
			{
				// Найден файл, который надо скопировать на внутреннюю флэшу
				if (strstr ("MASTER.BIN", fileInfo.fname))
				{
					slotNum = 0;
					dprintf ("This is new firmware for master. Send it to special slot and reflash himself\r\n");
					res = f_open (&file, fileInfo.fname, FA_READ);
					if (res == FR_OK) sendFileToFlash (&file, fileInfo.fname, slotNum);

				}
				else if (strstr ("SLAVE.BIN", fileInfo.fname))
				{
					slotNum = 0;
					dprintf ("This is new firmware for slave. Send it to special slot and store for sending\r\n");				
					res = f_open (&file, fileInfo.fname, FA_READ);
					if (res == FR_OK) sendFileToFlash (&file, fileInfo.fname, slotNum);
				}
				else
				{
					sscanf (fileInfo.fname, "SCENE%d", &slotNum);
					dprintf ("This is scene file. Slot num %d\r\n", slotNum);
					res = f_open (&file, fileInfo.fname, FA_READ);
					if (res == FR_OK) sendFileToFlash (&file, fileInfo.fname, slotNum);
				}				
			}
			else
			{
				dprintf ("Not applicable\r\n");
			}
		}
		f_closedir (&dir);
	}	
	return res;
}

static void sendFileToFlash (FIL *file, char *fname, uint8_t slot)
{
	UINT br = 0;
	
	dprintf ("\x1b[36mFile size %d bytes. Store in the flash slot %d...\x1b[0m\r\n", file->fsize, slot);
	dprintf ("\x1b[36mClean the slot...\x1b[0m");
	fileStorage_erase (slot);
	dprintf ("\x1b[36m done\x1b[0m\r\n");
	
	do 
	{
		f_read (file, buff, sizeof (buff), &br);
		if (br) 
		{
			fileStorage_write (slot, (uint8_t*)buff, br);
		}
	} while (br > 0);
	fileStorage_finalize (slot, fname);
	dprintf ("\x1b[36mFile stored\x1b[0m\r\n");
}
