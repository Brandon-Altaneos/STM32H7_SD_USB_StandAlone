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
/*==                             error.c                                 ==*/
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

#include "errors.h"

/*====================================================================*/
/*==                       MACROS Definitions                       ==*/
/*====================================================================*/

/*====================================================================*/
/*==                       DATA STRUCTURES                          ==*/
/*====================================================================*/

/*====================================================================*/
/*==                        Global Variables                        ==*/
/*====================================================================*/

/*====================================================================*/
/*==              Private Functions Definitions                     ==*/
/*====================================================================*/

/*====================================================================*/
/*==               Public Functions Definitions                     ==*/
/*====================================================================*/

char* ERROR_ErrToStr(E_Error eCode)
{
	switch (eCode)
	{
		case ERR_SUCCESS:			return "SUCCESS";

		case ERR_OTHER:				return "Error: Other";
		case ERR_INVALID_ARG:		return "Error: Invalid argument";
		case ERR_NOT_IMPLEMENTED:	return "Error: Not implemented";
		case ERR_TIME_OUT:			return "Error: Time out";
		case ERR_NO_DEVICE:			return "Error: No device";
		case ERR_INVALID_CRC:		return "Error: CRC";
		case ERR_LIMIT_REACHED:		return "Error: Limit reached";
		case ERR_QUEUE_CREATE:		return "Error: Create queue";
		case ERR_TASK_CREATE:		return "Error: Create task";
		case ERR_UPDATED:			return "Param Updated";
		case ERR_BAD_VERSION:		return "Error: Bad version";
		case ERR_EMPTY:				return "Error: Buffer empty";
		case ERR_FULL:				return "Error: Buffer full";
		case ERR_INVALID_SIZE:		return "Error; Invalid size";

		case ERR_I2C_RX:			return "I2C: RX";
		case ERR_I2C_TX:			return "I2C: TX";
		case ERR_SPI:				return "SPI";
		case ERR_SPI_RX:			return "SPI: RX";
		case ERR_SPI_TX:			return "SPI: TX";

		case ERR_CRC:				return "CRC";
		case ERR_INVALID_DATA:		return "Invalid data";
		case ERR_UART_RX:			return "UART: RX";
		case ERR_UART_TX:			return "UART: TX";

		case ERR_FLASH_ERROR: 					return "FLASH: Error";
		case ERR_FLASH_BUSY:					return "FLASH: Busy";
		case ERR_FLASH_TIMEOUT:					return "FLASH: Timeout";
		case ERR_FLASH_READ_ID:					return "FLASH: Read ID";
		case ERR_FLASH_WRONG_ID:				return "FLASH: Wrong ID";
		case ERR_FLASH_QSPI_MODE:				return "FLASH: QSPI mode";
		case ERR_FLASH_MEMORY_MAPPED:			return "FLASH: Memory mapped";
		case ERR_FLASH_AUTO_POLLING_MEMORY:		return "FLASH: Auto polling memory";
		case ERR_FLASH_NOT_FOUND:				return "FLASH: Not found";

		case ERR_SD_CARD_NOT_PRESENT:			return "SD CARD: Not present";
		case ERR_SD_CARD_WRITE_ENABLE:			return "SD CARD: Write not enable";

		case ERR_FATFS_CREATE:		return "FATFS: Create";
		case ERR_FATFS_MOUNT:		return "FATFS: Mount";
		case ERR_FATFS_WRITE:		return "FATFS: Write";
		case ERR_FATFS_READ:		return "FATFS: Read";
		case ERR_FATFS_FILE_NOT_EXIST:	return "FATFS: File not exist";
		case ERR_FATFS_EMPTY:		return "FATFS: Empty";
		case ERR_FATFS_NEW_SD:		return "FATFS: New SD";

		case ERR_DAT_OTHER:			return "DAT: Other";
		case ERR_DAT_WATCHDOG:		return "DAT: Watchdog";
		case ERR_DAT_LOG:			return "DAT: Log";
		case ERR_DAT_FLASH:			return "DAT: Flash";
		case ERR_DAT_SPICPU:		return "DAT: SPI CPU";

		case ERR_COM_OTHER:			return "COM: Other";
		case ERR_COM_FORMAT:		return "COM: Format";
		case ERR_COM_PAYLEN:		return "COM: Payload len";
		case ERR_COM_CRC:			return "COM: CRC";
		case ERR_COM_BAD_COMMAND:	return "COM: Bad command";

		case ERR_MOTOR_WRITE_REGISTER:	return "MOTOR: Write register";
		case ERR_MOTOR_READ_REGISTER:	return "MOTOR: Read register";
		case ERR_MOTOR_MOTION:			return "MOTOR: Already moving";
		case ERR_MOTOR_POSITION:		return "MOTOR: Already in position";
		case ERR_MOTOR_DURATION:		return "MOTOR: No duration";
		case ERR_MOTOR_BDU_UNPLUG:		return "MOTOR: BDU is not plug";
		case ERR_MOTOR_STALL:			return "MOTOR: Stall";
		case ERR_MOTOR_FORCE_MOVEMENT:	return "MOTOR: Force movement";
		case ERR_MOTOR_TEMPERATURE:		return "MOTOR: Temperature";

		case ERR_FWUP_OTHER:		return "FWUP: Other";
		case ERR_FWUP_SIZE:			return "FWUP: Size";
		case ERR_FWUP_CRC:			return "FWUP: CRC";
		case ERR_FWUP_NOTVALID:		return "FWUP: Not valid";
		case ERR_FWUP_FLASH:		return "FWUP: Flash";
		case ERR_FWUP_NOTPREPARED:	return "FWUP: Not prepared";

		case ERR_CONTINUE:			return "Continue";

		default:            		return "*unknown code*";
	}
}
