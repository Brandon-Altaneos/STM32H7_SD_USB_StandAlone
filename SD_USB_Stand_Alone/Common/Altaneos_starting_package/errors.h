/*==*********************************************************************==*/
/*=*                                                                     *=*/
/*=*                         ALTANEOS S.R.L.                             *=*/
/*=*   ==                                                           ==   *=*/
/*=*   ===============================================================   *=*/
/*=*                                                                     *=*/
/*=*                        Chaussee Verte, 93b                          *=*/
/*=*                  4370 SAINT-GEORGES-SUR-MEUSE                       *=*/
/*=*                              BELGIUM                                *=*/
/*=*                                                                     *=*/
/*=*     Tel: +32.4.233 37 03                                            *=*/
/*=*     Web: http://www.altaneos.com                                    *=*/
/*=*     Email: jb@altaneos.com                                          *=*/
/*=*                                                                     *=*/
/*==*********************************************************************==*/
/*=========================================================================*/
/*==                                                                     ==*/
/*==                             error.h                                 ==*/
/*==                                                                     ==*/
/*==---------------------------------------------------------------------==*/
/*==                                                                     ==*/
/*==   Client Company          : IBA                                     ==*/
/*==   Project Name            : E-Source V2                             ==*/
/*==   Project Internal Code   : PRJ00762                                ==*/
/*==   Author(s)               : Biemar Jean                             ==*/
/*==   Date of creation        : 12/09/2023                              ==*/
/*==   Coding Scheme           : Altaneos Coding Conventions 			 ==*/
/*== 							 V2.0 20210407            				 ==*/
/*==                                                                     ==*/
/*== File Description :                                                  ==*/
/*== 	Enumeration of all possible errors 		 						 ==*/
/*==                                                                     ==*/
/*==-------------------------------------------------------------------  ==*/
/*==                                                                     ==*/
/*== Unit Version			   : 1.0                                     ==*/
/*==-------------------------------------------------------------------  ==*/
/*==                                                                     ==*/
/*== Notes:                                                              ==*/
/*==-------------------------------------------------------------------  ==*/
/*==                                                                     ==*/
/*== File Modifications :                                                ==*/
/*==-------------------------------------------------------------------  ==*/
/*==     	                                                             ==*/
/*=========================================================================*/

#ifndef _ERRORS_H_
#define _ERRORS_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*====================================================================*/
/*==                           INCLUDES                             ==*/
/*====================================================================*/

/*====================================================================*/
/*==                       MACROS Definitions                       ==*/
/*====================================================================*/

/*====================================================================*/
/*==                       DATA STRUCTURES                          ==*/
/*====================================================================*/

typedef enum
{
	ERR_SUCCESS                         = 0x00,
	
	ERR_OTHER							= 0x01,
	ERR_INVALID_ARG						= 0x02,
	ERR_NOT_IMPLEMENTED                 = 0x03,
	ERR_TIME_OUT						= 0x04,
	ERR_NO_DEVICE						= 0x05,
	ERR_INVALID_CRC						= 0x06,
	ERR_LIMIT_REACHED					= 0x07,
	ERR_QUEUE_CREATE					= 0x08,
	ERR_TASK_CREATE						= 0x09,
	ERR_UPDATED							= 0x0a,
	ERR_BAD_VERSION						= 0x0b,
	ERR_EMPTY							= 0x0c,
	ERR_FULL							= 0x0d,
	ERR_INVALID_SIZE					= 0x0e,
	ERR_NOT_INIT						= 0x0f,

	ERR_I2C_RX							= 0x10,
	ERR_I2C_TX							= 0x11,
	ERR_SPI								= 0x12,
	ERR_SPI_RX							= 0x13,
	ERR_SPI_TX							= 0x14,

	ERR_CRC								= 0x20,
	ERR_INVALID_DATA					= 0x21,

	ERR_UART_RX							= 0x30,
	ERR_UART_TX							= 0x31,

	ERR_FLASH_ERROR 					= 0x40,
	ERR_FLASH_BUSY						= 0x41,
	ERR_FLASH_TIMEOUT					= 0x42,
	ERR_FLASH_READ_ID					= 0x43,
	ERR_FLASH_WRONG_ID					= 0x44,
	ERR_FLASH_QSPI_MODE					= 0x45,
	ERR_FLASH_MEMORY_MAPPED				= 0x46,
	ERR_FLASH_AUTO_POLLING_MEMORY		= 0x47,
	ERR_FLASH_NOT_FOUND					= 0x48,

	ERR_SD_CARD_NOT_PRESENT				= 0x50,
	ERR_SD_CARD_WRITE_ENABLE			= 0x51,

	ERR_FATFS_CREATE                    = 0x60,
	ERR_FATFS_MOUNT                     = 0x61,
	ERR_FATFS_WRITE                     = 0x62,
	ERR_FATFS_READ                      = 0x63,
	ERR_FATFS_FILE_NOT_EXIST            = 0x64,
	ERR_FATFS_EMPTY                     = 0x65,
	ERR_FATFS_NEW_SD                    = 0x66,

	ERR_START_OTHER						= 0x70,
	ERR_START_WATCHDOG					= 0x71,
	ERR_START_LOG						= 0x72,
	ERR_START_ALARM						= 0x73,
	ERR_START_FLASH						= 0x74,
	ERR_START_SPICPU					= 0x75,
	ERR_START_RTC						= 0x76,
	ERR_START_TEMP						= 0x77,
	ERR_START_BATTERY					= 0x78,
	ERR_START_SENSORS					= 0x79,
	ERR_START_MOTOR						= 0x7A,

	ERR_DAT_OTHER						= 0x80,
	ERR_DAT_WATCHDOG					= 0x81,
	ERR_DAT_LOG							= 0x82,
	ERR_DAT_FLASH						= 0x83,
	ERR_DAT_SPICPU						= 0x84,

	ERR_COM_OTHER						= 0xb0,
	ERR_COM_FORMAT						= 0xb1,
	ERR_COM_PAYLEN						= 0xb2,
	ERR_COM_CRC							= 0xb3,
	ERR_COM_BAD_COMMAND					= 0xb4,
	
	ERR_MOTOR_WRITE_REGISTER			= 0xc0,
	ERR_MOTOR_READ_REGISTER				= 0xc1,
	ERR_MOTOR_MOTION					= 0xc2,
	ERR_MOTOR_POSITION					= 0xc3,
	ERR_MOTOR_DURATION					= 0xc4,
	ERR_MOTOR_BDU_UNPLUG				= 0xc5,
	ERR_MOTOR_BDU_NOT_ENGAGED			= 0xc6,
	ERR_MOTOR_STALL						= 0xc7,
	ERR_MOTOR_FORCE_MOVEMENT			= 0xc8,
	ERR_MOTOR_TEMPERATURE				= 0xc9,

	ERR_FWUP_OTHER						= 0xe0,
	ERR_FWUP_SIZE						= 0xe1,
	ERR_FWUP_CRC						= 0xe2,
	ERR_FWUP_NOTVALID					= 0xe3,
	ERR_FWUP_FLASH						= 0xe4,
	ERR_FWUP_NOTPREPARED				= 0xe5,

	ERR_CONTINUE						= 0xf0,

} E_Error;

/*====================================================================*/
/*==                        Exported Data                           ==*/
/*====================================================================*/


/*====================================================================*/
/*==                      Exported Functions                        ==*/
/*====================================================================*/

char* ERROR_ErrToStr(E_Error eCode);

#ifdef __cplusplus
}
#endif

#endif /* _ERRORS_H_ */
