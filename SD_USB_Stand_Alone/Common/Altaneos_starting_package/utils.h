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
/*==                               utils.h                               ==*/
/*==                                                                     ==*/
/*==---------------------------------------------------------------------==*/
/*==                                                                     ==*/
/*==   Client Company          : Achille                                 ==*/
/*==   Project Name            : Ecmo                                    ==*/
/*==   Project Internal Code   : PRJ00877                                ==*/
/*==   Author(s)               : Mbanzulu Lelamu Brandon                 ==*/
/*==   Date of creation        : 17/02/2025                              ==*/
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

#ifndef _UTILS_H_
#define _UTILS_H_

/*
** Include
*/

#include "globals.h"
#include "stdint.h"
#include "stdbool.h"
#include <time.h>
#include "stm32h7xx_hal.h"

/*
** Macro Definitions
*/

#define UTILS_IS_IN_INTERRUPT()                 (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk)
#define UTILS_ROUND_FLOAT_TO_INT32(x)           ((int32_t)((x) + ((x) >= 0 ? 0.5 : -0.5)))
#define UTILS_ROUND_FLOAT_TO_UINT32(x)          ((uint32_t)((x) + 0.5))
#define UTILS_ROUND_FLOAT_TO_INT16(x)           ((int16_t)((x) > INT16_MAX ? INT16_MAX : ((x) < INT16_MIN ? INT16_MIN : ((x) + ((x) >= 0 ? 0.5 : -0.5)))))
#define UTILS_ROUND_FLOAT_TO_UINT16(x)          ((uint16_t)((x) > UINT16_MAX ? UINT16_MAX : ((x) < 0 ? 0 : ((x) + 0.5))))
#define UTILS_KELVIN_TO_CELSIUS_FLOAT(x)        ((x) - 273.15f)
#define UTILS_CELSIUS_TO_KELVIN_FLOAT(x)        ((x) + 273.15f)
#define UTILS_KELVIN_TO_CELSIUS_INT(x)          ((x) - 273)
#define UTILS_CELSIUS_TO_KELVIN_INT(x)          ((x) + 273)

// Screen Start

/*
** Type Definitions
*/

/*
** Exported Functions
*/

int8_t UTILS_GetRandomInt8(int8_t min, int8_t max);
int16_t UTILS_GetRandomInt16(int16_t min, int16_t max);
int32_t UTILS_GetRandomInt32(int32_t min, int32_t max);
uint8_t UTILS_GetRandomUint8(uint8_t min, uint8_t max);
uint16_t UTILS_GetRandomUint16(uint16_t min, uint16_t max);
uint32_t UTILS_GetRandomUint32(uint32_t min, uint32_t max);
void UTILS_Uint32ToBinary(uint32_t iValue, char* szBinFormat);
void UTILS_Uint16ToBinary(uint16_t iValue, char* szBinFormat);
void UTILS_Uint8ToBinary(uint32_t iValue, char* szBinFormat);

#endif /* _UTILS_H_ */
