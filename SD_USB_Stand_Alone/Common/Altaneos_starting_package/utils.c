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
/*==                               utils.c                               ==*/
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

/*
** Include section
*/
#include "utils.h"

#include <stdlib.h>

/*
** Macro Definitions
*/
#define LOG_MODULE "UTILS"

/*
** Type Definitions
*/

/*
** Exported Global Data
*/

/*
** File Data
*/

/*
** Local Function Prototypes
*/

/*
** Function definitions
*/

int8_t UTILS_GetRandomInt8(int8_t min, int8_t max)
{
    if (min > max)
    {
        int16_t temp = min;
        min = max;
        max = temp;
    }
    
    return (int8_t)(min + (rand() % (max - min + 1)));
}

int16_t UTILS_GetRandomInt16(int16_t min, int16_t max)
{
    if (min > max)
    {
        int16_t temp = min;
        min = max;
        max = temp;
    }
    
    return (int16_t)(min + (rand() % (max - min + 1)));
}

int32_t UTILS_GetRandomInt32(int32_t min, int32_t max)
{
    if (min > max)
    {
        int32_t temp = min;
        min = max;
        max = temp;
    }
    
    return (int32_t)(min + (rand() % (max - min + 1)));
}

uint8_t UTILS_GetRandomUint8(uint8_t min, uint8_t max)
{
    if (min > max)
    {
        uint8_t temp = min;
        min = max;
        max = temp;
    }
    
    return (uint8_t)(min + (rand() % (max - min + 1)));
}

uint16_t UTILS_GetRandomUint16(uint16_t min, uint16_t max)
{
    if (min > max)
    {
        uint16_t temp = min;
        min = max;
        max = temp;
    }
    
    return (uint16_t)(min + (rand() % (max - min + 1)));
}

uint32_t UTILS_GetRandomUint32(uint32_t min, uint32_t max)
{
    if (min > max)
    {
        uint32_t temp = min;
        min = max;
        max = temp;
    }
    
    return (uint32_t)(min + (rand() % (max - min + 1)));
}

void UTILS_Uint32ToBinary(uint32_t iValue, char* szBinFormat)
{
    for (int i = 31; i >= 0; i--) {
        szBinFormat[i] = (iValue & (1U << i)) ? '1' : '0';
    }
    szBinFormat[32] = '\0';
}

void UTILS_Uint16ToBinary(uint16_t iValue, char* szBinFormat)
{
    for (int i = 15; i >= 0; i--) {
        szBinFormat[i] = (iValue & (1U << i)) ? '1' : '0';
    }
    szBinFormat[16] = '\0';
}

void UTILS_Uint8ToBinary(uint32_t iValue, char* szBinFormat)
{
    for (int i = 7; i >= 0; i--) {
        szBinFormat[i] = (iValue & (1U << i)) ? '1' : '0';
    }
    szBinFormat[8] = '\0';
}
