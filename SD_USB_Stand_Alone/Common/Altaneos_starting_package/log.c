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
/*==                             log.c                                   ==*/
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
/*== 	Write log to the Debug UART		 						 		 ==*/
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

#include "log.h"
#include <string.h>

#ifdef LOG_ENABLED

/*====================================================================*/
/*==                       MACROS Definitions                       ==*/
/*====================================================================*/

#if LOG_COLOR

#define BOLD  "\x1b[1m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#else
#define BOLD
#define RED
#define GRN
#define YEL
#define BLU
#define MAG
#define CYN
#define WHT
#define RESET

#endif

#define LOG_MAX_INTERRUPT_WAITING 5

/*====================================================================*/
/*==                       DATA STRUCTURES                          ==*/
/*====================================================================*/

/*====================================================================*/
/*==                        Global Variables                        ==*/
/*====================================================================*/
static E_LogSeverity eDebugLevel;

static UART_HandleTypeDef *loguart;

static char tmpMsgStringInterrupt[LOG_MSG_STRING_MAX_SIZE];
static char tmpLogStringInterrupt[LOG_MAX_INTERRUPT_WAITING][LOG_TOTAL_MAX_SIZE];
static uint8_t isLogInterruptWaiting = 0;

/*====================================================================*/
/*==              Private Functions Definitions                     ==*/
/*====================================================================*/

static char* LOG_SeverityToString(E_LogSeverity eSeverity)
{
	switch (eSeverity)
	{
	case E_LOGSEV_ERROR:
		return BOLD RED "ERROR" RESET;
	case E_LOGSEV_WARNING:
		return BOLD YEL "WARN " RESET;
	case E_LOGSEV_INFO:
		return BOLD GRN "INFO " RESET;
	case E_LOGSEV_DEBUG:
		return BOLD BLU "DEBUG" RESET;
	default:
		return "(-----)";
	}
}

/*====================================================================*/
/*==               Public Functions Definitions                     ==*/
/*====================================================================*/

E_Error LOG_Init(UART_HandleTypeDef *hUART, E_LogSeverity eLevel)
{
	if (hUART == NULL)
	{
		return ERR_INVALID_ARG;
	}
	loguart = hUART;

	eDebugLevel = eLevel;

	LOG_Send("LOG", E_LOGSEV_INFO, "%s: LOG initialized with level %s", __func__, LOG_SeverityToString(eDebugLevel));

	return ERR_SUCCESS;
}

void LOG_Send(const char* szModule, E_LogSeverity eSeverity,const char* pMsg, ...)
{
	if(UTILS_IS_IN_INTERRUPT())
	{
		return;
		va_list args;

		if (eSeverity > eDebugLevel)
			return;

		if(isLogInterruptWaiting >= LOG_MAX_INTERRUPT_WAITING)
		{
			isLogInterruptWaiting = LOG_MAX_INTERRUPT_WAITING+1;
		}

		va_start(args, pMsg);
		vsnprintf(tmpMsgStringInterrupt, LOG_MSG_STRING_MAX_SIZE - 1, pMsg, args);
		va_end(args);

		snprintf(tmpLogStringInterrupt[isLogInterruptWaiting], LOG_TOTAL_MAX_SIZE - 1, BOLD "[%010lu]" RESET " %s - ISR " BOLD "%s" RESET ": %s\r\n", HAL_GetTick(), LOG_SeverityToString(eSeverity), szModule, tmpMsgStringInterrupt);

		isLogInterruptWaiting++;
	}
	else
	{
		char tmpMsgString[LOG_MSG_STRING_MAX_SIZE];
		char tmpLogString[LOG_TOTAL_MAX_SIZE];
		va_list args;

		if (eSeverity > eDebugLevel)
			return;

		va_start(args, pMsg);
		vsnprintf(tmpMsgString, LOG_MSG_STRING_MAX_SIZE - 1, pMsg, args);
		va_end(args);

		snprintf(tmpLogString, LOG_TOTAL_MAX_SIZE - 1, BOLD "[%010lu]" RESET " %s - " BOLD "%s" RESET ": %s\r\n", HAL_GetTick(), LOG_SeverityToString(eSeverity), szModule, tmpMsgString);

		HAL_UART_Transmit(loguart, (uint8_t*) tmpLogString, strlen(tmpLogString), LOG_SEND_TIMEOUT);
	}
}

#endif
