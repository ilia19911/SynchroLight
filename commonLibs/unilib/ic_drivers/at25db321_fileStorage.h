#ifndef _AT25DB321_FILESTORAGE_H_
#define _AT25DB321_FILESTORAGE_H_

#define FS_SECTORS_NUM								64
#define FS_SLOT_SIZE_SECTORS					8
#define FS_BLOCK_SIZE_BYTES						4096
#define FS_IO_BUFFER_SIZE							256
#define FS_SECTOR_SIZE								65536

#define FS_SLOTS_NUM									(FS_SECTORS_NUM / FS_SLOT_SIZE_SECTORS)

#define FS_SLOT_ALL										((uint8_t)0xff)

typedef struct
{
	uint32_t fileSize;
	uint32_t lastModify;
	uint32_t crc32;
	char fileName[16];
} at25db321_fileStorage_header_t;

typedef struct
{
	at25db321_fileStorage_header_t header;
	uint8_t finalize;
	uint8_t isOpen;
} at25db321_fileStorage_slot_t;

typedef struct
{
	at25db321_fileStorage_slot_t slot[FS_SLOTS_NUM];
	uint8_t outBuffer[FS_IO_BUFFER_SIZE];
	uint8_t *outBufpnt;
	uint32_t outputAddress;
	uint8_t lastSlotActivity;
} at25db321_fileStorage_t;

void fileStorageInit (void);
void fileStorage_erase (uint8_t slot);
void fileStorage_write (uint8_t slot, uint8_t *data, uint16_t length);
void fileStorage_finalize (uint8_t slot, char *fname);
uint8_t fileStorage_read (uint8_t slot, uint8_t *buffer, uint32_t offset, uint16_t length);
uint32_t fileStorage_getFileLength (uint8_t slot);
uint32_t fileStorage_getFileCRC (uint8_t slot);
uint32_t fileStorage_getFileData (uint8_t slot);
char* fileStorage_getFileName (uint8_t slot);
uint8_t fileStorage_getFreeSlot (void);
uint8_t fileStorage_isOpen (void);
int8_t fileStorage_activeSlot (void);

#endif /* _AT25DB321_FILESTORAGE_H_ */
