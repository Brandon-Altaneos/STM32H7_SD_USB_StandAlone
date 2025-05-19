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
/*=*     Email: bm@altaneos.com                                          *=*/
/*=*                                                                     *=*/
/*==*********************************************************************==*/
/*=========================================================================*/
/*==                                                                     ==*/
/*==                              usbdmsc.h                              ==*/
/*==                                                                     ==*/
/*==---------------------------------------------------------------------==*/
/*==                                                                     ==*/
/*==   Client Company          : Achille                                 ==*/
/*==   Project Name            : Ecmo                                    ==*/
/*==   Project Internal Code   : PRJ00877                                ==*/
/*==   Author(s)               : Mbanzulu Lelamu Brandon                 ==*/
/*==   Date of creation        : 04/02/2025                              ==*/
/*==   Coding Scheme           : Altaneos Coding Conventions 			 ==*/
/*== 							 V2.0 20210407            				 ==*/
/*==                                                                     ==*/
/*== File Description :                                                  ==*/
/*== 	                                 		 						 ==*/
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

#ifndef _USB_H_
#define _USB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Includes
*/

#include "globals.h"

/*
** Macro Definitions
*/

/*
** Type Definitions
*/

/*
** Exported Functions
*/

int8_t USBDMSC_Init_HS(uint8_t iLun);
int8_t USBDMSC_GetCapacity_HS(uint8_t iLun, uint32_t *iBlock_num, uint16_t *iBlock_size);
int8_t USBDMSC_IsReady_HS(uint8_t iLun);
int8_t USBDMSC_IsWriteProtected_HS(uint8_t iLun);
int8_t USBDMSC_Read_HS(uint8_t iLun, uint8_t *pBuf, uint32_t iBlk_addr, uint16_t iBlk_len);
int8_t USBDMSC_Write_HS(uint8_t iLun, const uint8_t *pBuf, uint32_t iBlk_addr, uint16_t iBlk_len);
int8_t USBDMSC_GetMaxiLun_HS(void);

void USBMSC_ConnectCallback(void);
void USBMSC_DisconnectCallback(void);

// Callback bsp_driver_sd DMA

void USBDMSC_BSP_SD_WriteCpltCallback(void);
void USBDMSC_BSP_SD_ReadCpltCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __USB_H__ */