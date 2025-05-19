#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include "globals.h"
#include "errors.h"
#include "log.h"
#include "ff.h"

#ifndef BYPASS_SD_CARD_DEBUG
/*====================================================================*/
/*==                       MACROS Definitions                       ==*/
/*====================================================================*/

#define SD_CARD_DIR_FW_PRO          "PRO-CPU_FW"
#define SD_CARD_DIR_FW_DAT          "DAT-CPU_FW"
#define SD_CARD_DIR_RECORDS         "Records"
#define SD_CARD_DIR_AVG_DATA        "Records/AVG"
#define SD_CARD_DIR_RAW_DATA        "Records/RAW"


#define SD_CARD_FW_SIZE            30

/*====================================================================*/
/*==                           INCLUDES                            ==*/
/*====================================================================*/

/*====================================================================*/
/*==                       DATA STRUCTURES                          ==*/
/*====================================================================*/

typedef enum SD_CARD_State_e
{
    SD_CARD_STATE_UNLOCK,
    SD_CARD_STATE_LOCK_FATFS,
    SD_CARD_STATE_LOCK_USBMSC
} SD_CARD_State_e;

/*====================================================================*/
/*==                        Exported Data                           ==*/
/*====================================================================*/


/*====================================================================*/
/*==                      Exported Functions                        ==*/
/*====================================================================*/

E_Error SD_CARD_InitCard(void);
E_Error SD_CARD_LinkDriver(void);
void SD_CARD_Deinit(void);
E_Error SD_CARD_FormatDisk(void);
E_Error SD_CARD_WriteSamplePacket(char *szFilename, uint8_t *pBuf, uint16_t iBufLen);
E_Error SD_CARD_WriteIndexesPacket(char *szFilename, uint8_t *pBuf, uint16_t iBufLen);
E_Error SD_CARD_WriteFirmware(char *szFilename, uint8_t *pBuf, uint16_t iBufLen);
FIL* SD_CARD_GetNewFileOpen(char *szFilename);
FIL* SD_CARD_GetFileOpen(char *szFilename);
void SD_CARD_GetFileClose(FIL *pFile);
uint32_t SD_CARD_GetFileSize(char *szFilename);
E_Error SD_CARD_DeleteFile(char *szFilename);
E_Error SD_CARD_DeleteFirmware(char *szFilename);
FRESULT SD_CARD_ScanAllFiles(char * path);
E_Error SD_CARD_SetFWValid(char *szFilename);
E_Error SD_CARD_GetDATFWName(char * szBuf);
bool SD_CARD_GetDATFWValid(void);
E_Error SD_CARD_GetPROFWName(char * szBuf);
bool SD_CARD_GetPROFWValid(void);
uint32_t SD_CARD_GetFWSize(char* szPath); // return 0 if error
E_Error SD_CARD_GetFWBlock(uint32_t iBlockNumber, uint16_t iBlockSize, uint8_t *pBlock, char* szPath);
E_Error SD_CARD_IsPresent(void);
E_Error SD_CARD_IsWriteEnable(void);
E_Error SD_CARD_IsReady(void);
bool SD_CARD_IsBspInit(void);
int8_t SD_CARD_BspInit(void);
SD_CARD_State_e SD_CARD_GetState(void);
void SD_CARD_SetState(SD_CARD_State_e eState);
void SD_CARD_UnitTest(void);

#else
#define SD_CARD_InitCard() ERR_SUCCESS
#define SD_CARD_LinkDriver() ERR_SUCCESS
#define SD_CARD_Deinit()
#define SD_CARD_FormatDisk() ERR_SUCCESS
#define SD_CARD_WriteAvgData(...) ERR_SUCCESS
#define SD_CARD_FormatAvfDataToBuffer(...) 0
#define SD_CARD_WriteAvgData(...) ERR_SUCCES
#define SD_CARD_WriteRawData(...) ERR_SUCCES
#define SD_CARD_WriteSamplePacket(...) ERR_SUCCESS
#define SD_CARD_WriteIndexesPacket(...) ERR_SUCCESS
#define SD_CARD_WriteFirmware(...) ERR_SUCCESS
#define SD_CARD_GetFileOpen(...) NULL
#define SD_CARD_GetFileClose(...)
#define SD_CARD_GetFileSize(...) 0
#define SD_CARD_DeleteFile(...) ERR_SUCCESS
#define SD_CARD_DeleteFirmware(...) ERR_SUCCESS
#define SD_CARD_ScanAllFiles(...) 0
#define SD_CARD_SetFWValid(...) ERR_SUCCESS
#define SD_CARD_GetCOMFWName(...) ERR_SUCCESS
#define SD_CARD_GetCOMFWValid(...) true
#define SD_CARD_GetNVRAMName(...) ERR_SUCCESS
#define SD_CARD_GetNVRAMValid(...) true
#define SD_CARD_GetPROFWName(...) ERR_SUCCESS
#define SD_CARD_GetPROFWValid(...) true
#define SD_CARD_GetFWSize(...) 0
#define SD_CARD_GetFWBlock(...) ERR_SUCCESS
#define SD_CARD_IsPresent()
#define SD_CARD_IsWriteEnable()
#define SD_CARD_IsReady()
#define SD_CARD_IsBspInit() 0
#define SD_CARD_BspInit() 0
#define SD_CARD_GetState() 0
#define SD_CARD_SetState(...);
#define SD_CARD_UnitTest()
#endif

#endif /* _SD_CARD_H_ */

