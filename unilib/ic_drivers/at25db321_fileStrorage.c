#include "include.h"

at25db321_fileStorage_t fStorage;

static void fileStorage_flush_outBuffer (uint8_t slot);
static void fileStorage_write_header (uint8_t slot);
static int8_t curSlot = -1;
static uint8_t fileIsOpen = 0;

static uint32_t timestamp = 0;

void fileStorageInit ()
{
	uint8_t k;
	uint32_t addr;
	
	fStorage.outBufpnt = fStorage.outBuffer;
	dprintf ("========= File storage =========\r\n");
	
	// Нужно считать хидеры в начале каждого слота.
	for (k = 0; k < FS_SLOTS_NUM; k++)
	{
		addr = (k * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS);
		at25df_read (addr, (void*)&fStorage.slot[k].header, sizeof (at25db321_fileStorage_header_t));
		fStorage.slot[k].finalize = 0;
		fStorage.slot[k].isOpen = 0;
		dprintf ("Slot %d\t", k+1);
		
		if (fStorage.slot[k].header.fileSize == 0xffffffff) 
		{
			fStorage.slot[k].header.fileSize = 0;
			fStorage.slot[k].header.lastModify = 0;
			dprintf ("None\r\n");
		}
		else 
		{
			fStorage.slot[k].finalize = 1;
			dprintf ("%s %d bytes\r\n", fStorage.slot[k].header.fileName, fStorage.slot[k].header.fileSize);
		}		
	}
	
	// После инициализации буфер записи пустой. Поэтому надо сбросить метку последней активности
	fStorage.lastSlotActivity = FS_SLOT_ALL;
	fStorage.outputAddress = 0x00;
}

void fileStorage_erase (uint8_t slot)
{
	uint8_t k;
	uint32_t addr;
	
	if (slot == FS_SLOT_ALL) at25df_erase ();
	else
	{
		if (slot < FS_SLOTS_NUM)
		{
				#ifdef DEBUG_FSTORAGE
				dprintf ("\x1b[1;36mClear slot %d\x1b[0m\n", slot);
				#endif /* DEBUG_FSTORAGE */

			// Тут надо вычислить адрес внутри блоков.
			for (k = 0; k < FS_SLOT_SIZE_SECTORS; k++)
			{
				addr = (slot * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS) + (k * FS_SECTOR_SIZE);
				at25df_erase_block64k (addr);
			}
		}
		fStorage.slot[slot].finalize = 0;
		fStorage.slot[slot].isOpen = 0;
		fStorage.slot[slot].header.fileName[0] = 0x00;
		fStorage.slot[slot].header.fileSize = 0x00;
	}
}

void fileStorage_write (uint8_t slot, uint8_t *data, uint16_t length)
{
	uint16_t lenToFull;
	uint16_t size;
	
	// Сначала посмотрим - какой слот был предыдущим
	if (fStorage.lastSlotActivity != slot)
	{
		// Писали в другой слот. Значит надо предыдущий сначала записать
		fileStorage_flush_outBuffer (fStorage.lastSlotActivity);
		
		// Запросим длину существующего файла
		fStorage.lastSlotActivity = slot;
		fStorage.outputAddress = (slot * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS) + FS_BLOCK_SIZE_BYTES + fStorage.slot[slot].header.fileSize;
		
		if (fStorage.lastSlotActivity != FS_SLOT_ALL) fStorage.slot[fStorage.lastSlotActivity].isOpen = 0;
	}
	
	fStorage.slot[slot].isOpen = 1;
	
	// Проверяем - сколько данных из вновь прибывших можно невозбранно затолкать в буфер
	while (length)
	{
		lenToFull = fStorage.outBuffer + sizeof (fStorage.outBuffer) - fStorage.outBufpnt;
		
		// Если влазит не все - то запишем столько, сколько влезет. И так будем кружить в цикле, пока не запишем все
		if (length > lenToFull) size = lenToFull;
		else size = length;
		
		// Копируем данные в буффер и двигаем указатели
		memcpy (fStorage.outBufpnt, data, size);
		fStorage.outBufpnt += size;
		data += size;
		
		// Если указатель в конце - флушим
		if (!(fStorage.outBuffer + sizeof (fStorage.outBuffer) - fStorage.outBufpnt)) fileStorage_flush_outBuffer (slot);
		
		length -= size;
	}
}

void fileStorage_finalize (uint8_t slot, char *fname)
{
	uint32_t addr, stopAddr;
	uint32_t crc;
	uint8_t buffer[256], isFirst;
	uint16_t size;
	
	fileStorage_flush_outBuffer (slot);

	if (fStorage.slot[slot].finalize) return;
	
	// Теперь считываем файл потихоньку в буффер и считаем его CRC32
	addr = (slot * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS) + FS_BLOCK_SIZE_BYTES;
	stopAddr = addr + fStorage.slot[slot].header.fileSize;
	
	isFirst = 1;
	
	while (addr < stopAddr)
	{
		if ((stopAddr - addr) < sizeof (buffer)) size = (stopAddr - addr);
		else size = sizeof (buffer);
		at25df_read (addr, (void*)buffer, size);
		
		if (isFirst)
		{
			crc = crc32hw (buffer, size, 1);
			isFirst = 0;
		}
		else
		{
			crc = crc32hw_append (crc, buffer, size);
		}
		addr += size;
	}
	
	// Установим црц32 для файла
	fStorage.slot[slot].header.crc32 = crc;
	
	// Установим флаги закрытия файла и его готовности к передаче
	fStorage.slot[slot].finalize = 1;
	fStorage.slot[slot].isOpen = 0;
	
	strcpy (fStorage.slot[slot].header.fileName, fname);
	
	fileStorage_write_header (slot);
}

static void fileStorage_flush_outBuffer (uint8_t slot)
{
		uint16_t size;
	
		// Посчитаем, сколько г..на у нас в буфере, подлежащего записи
		size = fStorage.outBufpnt - fStorage.outBuffer;
	
		if (!size) return;		// Если нечего сохранять - то отваливаемся

		#ifdef DEBUG_FSTORAGE
		dprintf ("\x1b[36mSave file buffer to FLASH\x1b[0m\n");
		dprintf ("\x1b[36mWriting %d bytes\tFrom addr %08x\x1b[0m\n", size, fStorage.outputAddress);
		#endif /* DEBUG_FSTORAGE */
	
		// Записываем данные по адресу последнего смещения
		at25df_write (fStorage.outputAddress, (void*)fStorage.outBuffer, size);
		fStorage.slot[slot].header.fileSize += size;
		fStorage.slot[slot].header.lastModify = timestamp;
		
		// Сбрасываем указатель внутри буфера на начало
		fStorage.outBufpnt = fStorage.outBuffer;
	
		// Двигаем указатель адреса
		fStorage.outputAddress += size;
	
		// Так как поменялось содержимое файло - надо сбросить флаг готовности файла
		fStorage.slot[slot].finalize = 0;
		#ifdef DEBUG_FSTORAGE
		dprintf ("\x1b[36mCurrant address %08x\x1b[0m\n", fStorage.outputAddress);
		#endif /* DEBUG_FSTORAGE */
	
	
//		// Перезапишем хидер
//		fileStorage_write_header (slot);
}

static void fileStorage_write_header (uint8_t slot)
{
		// Перезаписываем хидер слота
		at25df_erase_block4k ((slot * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS));
		at25df_write ((slot * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS), (void*)&fStorage.slot[slot].header, sizeof (at25db321_fileStorage_header_t));
	
		#ifdef DEBUG_FSTORAGE
		dprintf ("\x1b[36mWrite new header\x1b[0m\n");
		dprintf ("\x1b[36mFile name: %s\tFile size: %d bytes\tCRC: %08x\x1b[0m\n", fStorage.slot[slot].header.fileName, fStorage.slot[slot].header.fileSize, fStorage.slot[slot].header.crc32);
		#endif /* DEBUG_FSTORAGE */
}

uint8_t fileStorage_read (uint8_t slot, uint8_t *buffer, uint32_t offset, uint16_t length)
{
	uint32_t addr;
	uint8_t size;
	
	if (slot >= FS_SLOTS_NUM) return 0;
	if (offset + length > fStorage.slot[slot].header.fileSize) size = fStorage.slot[slot].header.fileSize - offset;
	else size = length;

	#ifdef DEBUG_FSTORAGE
	dprintf ("\x1b[36mRead from file\x1b[0m\n");
	dprintf ("\x1b[36mRead %d bytes with offset %d bytes\x1b[0m\n", length, offset);
	#endif /* DEBUG_FSTORAGE */

	
	addr = (slot * FS_SECTOR_SIZE * FS_SLOT_SIZE_SECTORS) + FS_BLOCK_SIZE_BYTES + offset;
	at25df_read (addr, buffer, size);
	
	return size;
}

uint8_t fileStorage_getFreeSlot (void)
{
	// Ищем пустой слот для записи
	uint8_t k;
	
	for (k = 0; k < FS_SLOTS_NUM; k++)
		if (fStorage.slot[k].header.fileSize == 0) break;
	
	if (k == FS_SLOTS_NUM) return 0xff;
	
	else return k;
}

uint32_t fileStorage_getFileLength (uint8_t slot)
{
	return fStorage.slot[slot].header.fileSize;
}

uint32_t fileStorage_getFileCRC (uint8_t slot)
{
	return fStorage.slot[slot].header.crc32;
}

uint32_t fileStorage_getFileData (uint8_t slot)
{
	return fStorage.slot[slot].header.lastModify;
}

char* fileStorage_getFileName (uint8_t slot)
{
	return fStorage.slot[slot].header.fileName;
}

uint8_t fileStorage_isOpen (void)
{
	return fileIsOpen;
}

int8_t fileStorage_activeSlot (void)
{
	return curSlot;
}
