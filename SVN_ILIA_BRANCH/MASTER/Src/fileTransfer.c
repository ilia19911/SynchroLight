#include "include.h"

static FATFS fs;
static char buff[256];

static void mounting (void);
//static void unmounting (void);
static FRESULT scan_files (char *path);
//static void sendFileToFlash (FIL *file, uint8_t slot);

void sdTask (void)
{
		mounting ();
}

static void mounting (void)
{
	FRESULT result;
	
	dprintf ("SD Card inserted\r\n");

	result = f_mount (&fs, "", 0);
	if (result != FR_OK)
	{
		dprintf ("Mount failure\r\n");
	}
	else 
	{
		dprintf ("Mount complete\r\n");
		strcpy (buff, "/");
		scan_files (buff);
	}
}

//static void unmounting (void)
//{
//	dprintf ("SD Card removed... Unmounting\r\n");
//}

static FRESULT scan_files (char *path)
{
	DIR dir;
	FILINFO fileInfo;
	FRESULT res;
	int slotNum;
	
	res = f_opendir (&dir, path);
	if (res == FR_OK)														
	{
		for (;;)
		{
			res = f_readdir (&dir, &fileInfo);										// Считываем директорию
			if (res != FR_OK || fileInfo.fname[0] == 0) break;		// Если какая ошибка - отваливаемся
			if (strstr (fileInfo.fname, ".BIN"))
			{
				// Найден файл, который надо скопировать на внутреннюю флэшу
				dprintf ("Found data file: %s\r\n", fileInfo.fname);
				if (strstr ("FIRMWARE", fileInfo.fname))
				{
					slotNum = 0;
					dprintf ("This is new firmware. Send it to special slot\r\n");
				}
				else
				{
					sscanf (fileInfo.fname, "SCENE%d", &slotNum);
					dprintf ("This is scene file. Slot num %d\r\n", slotNum);
				}
				
				
			}
		}
		f_closedir (&dir);
	}	
	return res;
}

//static void sendFileToFlash (FIL *file, uint8_t slot)
//{

//}
