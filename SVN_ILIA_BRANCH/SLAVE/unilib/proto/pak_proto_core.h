/* 
 * Pak protocol.
 *
 *  Serial port cfg:    115200-8-n-1
 *  Pack prefix:        xB0\x3B\x4F
 *  Fields format:      little-endian
 *  CRC16 (CCIIT):      init:0xffff, poly:?
 *
 */


#ifndef _PAK_PROTO_CORE_H_
#define _PAK_PROTO_CORE_H_

#define PAK_PROTO_UART                   uart_modem

#define PAK_PROTO_REV                     7
#define PAK_PROTO_REV_STR                "7.0"

#define PAK_PROTO_PREFIX_STR             "\xB0\x3B\x4F"
#define PAK_PROTO_PREFIX_SIZE            (sizeof(PAK_PROTO_PREFIX_STR)-1)

#define PAK_PROTO_CRC_SIZE               (sizeof(uint16_t))

#define PAK_PROTO_MAX_FRAME_SIZE         256

#define PAK_PROTO_MAX_DATA_SIZE          (PAK_PROTO_MAX_FRAME_SIZE - (sizeof(pak_proto_frame_header_t) - PAK_PROTO_PREFIX_SIZE) - PAK_PROTO_CRC_SIZE)  //  256-(9-3)-2 = 256-8 = 248

#define PAK_PROTO_HEADER_SIZE            sizeof(pak_proto_frame_header_t)

#define PAK_PROTO_BAD_FRAME_NUMBER       ((uint16_t)-1)

#define PAK_PROTO_ACK_BIT_MASK           (1<<7)     // msb of address

#define PAK_PROTO_BROADCAST_ADDRESS      0  // ���� ����� � ��� �������������
#define PAK_PROTO_NPU_ADDRESS            1  // ����� ���� (�������)
#define PAK_PROTO_LIMB_ADDRESS           2  // ��� �����  (������ �� �����)

enum
{
	PAK_OK = 0,                // 0
	PAK_ERR_DUP_PACK,          // 1 (получен пакет c предыдущим номером (повторно) - это собственно и не ошибка, но позволит оценить качество связи)
	PAK_ERR_CRC16,             // 2 (получен пакет c битым CRC16)
	PAK_ERR_BAD_DATA_SIZE,     // 3 (размер пакета не корелирует с размером поля данных в протоколе передачи точек)
	PAK_ERR_MEM_WR,            // 4 (ошибка записи в память)
	PAK_ERR_MEM_ER,            // 5 (ошибка стирания в память)
	PAK_ERR_MEM_NOT_FREE,      // 6 (попытка записи в занятую память)
	PAK_ERR_DATA_OUT_OF_RANGE, // 7 (число вне диапазона)
	PAK_ERR_UNKNOWN_FRAME_TYPE // 8 (неизвестный тип фрейма)
};

enum
{	                           
	PAK_PROTO_FRAME_IDENTIFY                      = 0, // � ����� �� ����:  ������ ������������� ����������
	PAK_PROTO_FRAME_PHOTO_STATUS                  = 1, // � ����� �� ����:  ������ ������� ������
	PAK_PROTO_FRAME_PHOTO_TEST_PICTURE            = 2, // � ����� �� ����:  ������ ��������� ������
	PAK_PROTO_FRAME_PHOTO_NEW_PICTURE             = 3, // � ����� �� �����: ����� �������
	PAK_PROTO_FRAME_PHOTO_CLR_TAGRETS             = 4, // � ����� �� ����:  �������� ������ ����
	PAK_PROTO_FRAME_PHOTO_TARGRT_LINE             = 5, // � ����� �� ����:  ������ ���� (����� ������ � ���. ����� ������ �� ������)
	PAK_PROTO_FRAME_PHOTO_SET_WIND_ENVIRONMENT    = 6, // 
	PAK_PROTO_FRAME_PHOTO_SET_OVERLAP_ENVIRONMENT = 7, // 
	PAK_PROTO_FRAME_PHOTO_SET_MODE                = 8,  // 
	PAK_PROTO_FRAME_RG_INFO						  = 9,	//
	PAK_PROTO_FRAME_DEL_REG_INFO				  = 10,	//
	PAK_PROTO_FRAME_PHOTO_FEEDFORWARD_DELAY		  = 11, //set the delay of photocamera, this sense use in coordinate esimator. Its time is advance 
	PAK_PROTO_FRAME_PHOTO_QTY_NON_LOAD			  = 12, //send to the 2DNPU tp show how many info of new picture not rx by 2DNPU
	PAK_PROTO_FRAME_START_EXCHNG				  = 13, //its cmd for AFS+OER version limb, Belarussia
	PAK_PROTO_FRAME_STOP_EXCHNG					  = 14,	//its cmd for AFS+OER version limb, Belarussia
	PAK_PROTO_FRAME_PHOTOCAM_ON_OFF				  = 15,	//its cmd for on/off photo
	PAK_PROTO_FRAME_SEND_ALT_LS1501				  = 16	//its cmd only for project arctica
};

#pragma pack(push, 1)


typedef	struct
{
	uint16_t type_photo;	//2
	uint16_t reserv1;		//4
	uint16_t reserv2;		//6
	uint16_t reserv3;		//8
}afs_service_info_t;

typedef	struct
{                                             // offset  size
	uint8_t  prefix[PAK_PROTO_PREFIX_SIZE];   // 0       7
	uint8_t  data_size;                       // 3       1
	uint8_t  not_data_size;                   // 4       1

	uint8_t  address;                         // 5       1
	uint8_t  frame_type;                      // 6       1
	uint16_t frame_number;                    // 7       2
} pak_proto_frame_header_t;

typedef struct
{
	pak_proto_frame_header_t  header;

	uint8_t                   buf[PAK_PROTO_MAX_FRAME_SIZE];
	int                       idx;

	int                       state;

	uint8_t*                  resync_buf;
	int                       resync_qty;

	uart_t*                   uart;

} pak_proto_handle_t;

#pragma pack(pop)


void pak_proto_init          (pak_proto_handle_t* h); // at25df_init() should be called fefore
void pak_proto_task          (pak_proto_handle_t* h);
void pak_proto_core_tx_frame (pak_proto_handle_t* h, pak_proto_frame_header_t* header, const void* data);

#endif
