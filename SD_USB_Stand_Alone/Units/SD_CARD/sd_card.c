#include "globals.h"
#include "sd_card.h"
#include "sd_diskio.h"
#include "bsp_driver_sd.h"
#include "ff_gen_drv.h"
#include "usbd_def.h"

#ifndef BYPASS_SD_CARD_DEBUG
/*====================================================================*/
/*==                       MACROS Definitions                       ==*/
/*====================================================================*/
#define LOG_MODULE "SD_CARD"

#define SD_CARD_PACKET_IN_BUFFER 20

#define PACKET_SIZE 104 // TODO: This is not clean but it's the only way I found to avoid bug in fatfs writing
#define CHUNK_SIZE 6656 // 64*104. 104b/0.008s = 13000b/s = 46800000b/h = +-50MB/h

#define WORK_BUFFER_SIZE    1024

/*====================================================================*/
/*==                       DATA STRUCTURES                          ==*/
/*====================================================================*/

/*====================================================================*/
/*==                        Global Variables                        ==*/
/*====================================================================*/

RAM_D1_NOCACHE static FATFS   SD_CARD_tFatFs;
RAM_D1_NOCACHE static char  SD_CARD_szPath[4];

#ifndef BOOTLOADER
RAM_D1_NOCACHE static uint8_t SD_CARD_pSampleBuffer[CHUNK_SIZE];
RAM_D1_NOCACHE static uint16_t SD_CARD_iSampleBufferCounter = 0;
RAM_D1_NOCACHE static char SD_CARD_szSampleSavedFileName[64] = {0};
RAM_D1_NOCACHE static char SD_CARD_szSampleActualFileName[64] = {0};
RAM_D1_NOCACHE static uint8_t SD_CARD_bFileNameValid = 0;
RAM_D1_NOCACHE static uint8_t SD_CARD_bNewFileName = 0;

RAM_D1_NOCACHE static char SD_CARD_pAvgBuffer[512] = {0};
RAM_D1_NOCACHE static uint16_t SD_CARD_iAvgBufferCounter = 0;
RAM_D1_NOCACHE static char SD_CARD_szAvgSavedFileName[64] = {0};
#endif

RAM_D1_NOCACHE static FIL SD_CARD_tFileRead;
RAM_D1_NOCACHE static FIL GL_SD_CARD_tFile;

RAM_D1_NOCACHE static uint8_t GL_SD_CARD_workBuffer[ _MAX_SS ];
RAM_D1_NOCACHE static char GL_SD_CARD_szWorkBuffer[1024] = {0};

static volatile int8_t GL_SD_CARD_iStatus;
static volatile bool bIsBspSdCardInit;
static uint8_t SD_CARD_bIsMounted = false;
SD_CARD_State_e volatile GL_SD_CARD_eState = SD_CARD_STATE_UNLOCK;

/*====================================================================*/
/*==              Private Functions Definitions                     ==*/
/*====================================================================*/

static E_Error SD_CARD_TestAndCreateFolder(char* path)
{
    DIR dir;
    if(f_opendir(&dir, path) != FR_OK)
    {
        if(f_mkdir(path) != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_mkdir %s Failed", __func__, path);
            return ERR_FATFS_CREATE;
        }
    }
    else
    {
        f_closedir(&dir);
    }
    return ERR_SUCCESS;
}

/*====================================================================*/
/*==               Public Functions Definitions                     ==*/
/*====================================================================*/
E_Error SD_CARD_InitCard(void)
{
    FRESULT res;
    E_Error err = ERR_SUCCESS;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    err = SD_CARD_IsPresent();
    if (err != ERR_SUCCESS)
    {
        return err;
    }

    err = SD_CARD_IsWriteEnable();
    if (err != ERR_SUCCESS)
    {
        return err;
    }

    // Check if there is a filesystem
    if(f_mount(&SD_CARD_tFatFs, (TCHAR const*)SD_CARD_szPath, 1) != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: No FS, create one", __func__);
        res = f_mkfs(SD_CARD_szPath, FM_ANY, 0, GL_SD_CARD_workBuffer, sizeof(GL_SD_CARD_workBuffer));
        if (res != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_mkfs Failed %i", __func__, res);
            return ERR_FATFS_CREATE;
        }
        if(f_mount(&SD_CARD_tFatFs, (TCHAR const*)SD_CARD_szPath, 0) != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_mount Failed %i", __func__, res);
            return ERR_FATFS_MOUNT;
        }
    }

    HAL_Delay(10);
    // Check folders
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_FW_PRO) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_FW_DAT) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_RECORDS) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_AVG_DATA) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_RAW_DATA) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: FS created and mounted", __func__);

/*
    SD_CARD_iSampleBufferCounter = 0;
    SD_CARD_szSampleSavedFileName[0] = 0;
    SD_CARD_szSampleActualFileName[0] = 0;
    SD_CARD_iAvgBufferCounter = 0;
    SD_CARD_szAvgSavedFileName[0] = 0;
  */  
    // unmount disk
    // f_mount(0, (TCHAR const*)SD_CARD_szPath, 0);

    SD_CARD_bIsMounted = true;
    SD_CARD_SetState(SD_CARD_STATE_LOCK_FATFS);

    return err;
}

E_Error SD_CARD_LinkDriver(void)
{
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    if(FATFS_LinkDriver(&SD_Driver, SD_CARD_szPath) != 0)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: FATFS_LinkDriver Failed", __func__);
        return ERR_FATFS_CREATE;
    }
    return ERR_SUCCESS;
}

void SD_CARD_Deinit(void)
{
    if(SD_CARD_bIsMounted)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);
        f_mount(0, (TCHAR const*)SD_CARD_szPath, 0);
        //FATFS_UnLinkDriver(SD_CARD_szPath);
        SD_CARD_bIsMounted = false;
    }
    SD_CARD_SetState(SD_CARD_STATE_UNLOCK);
}

E_Error SD_CARD_FormatDisk(void)
{
    FRESULT res;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);
    res = f_mkfs(SD_CARD_szPath, FM_FAT32, 0, GL_SD_CARD_workBuffer, sizeof(GL_SD_CARD_workBuffer));
    if (res != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_mkfs Failed %i", __func__, res);
        return ERR_FATFS_CREATE;
    }
    if(f_mount(&SD_CARD_tFatFs, (TCHAR const*)SD_CARD_szPath, 0) != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_mount Failed %i", __func__, res);
        return ERR_FATFS_MOUNT;
    }

    // Check folders
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_FW_PRO) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_FW_DAT) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_RECORDS) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_AVG_DATA) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;
    if(SD_CARD_TestAndCreateFolder(SD_CARD_DIR_RAW_DATA) != ERR_SUCCESS)
        return ERR_FATFS_CREATE;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: FS created and mounted", __func__);

    return ERR_SUCCESS;
}

#pragma GCC push_options
#pragma GCC optimize ("O0")

E_Error SD_CARD_WriteFirmware(char *szFilename, uint8_t *pBuf, uint16_t iBufLen)
{
    FRESULT res = FR_OK;
    FIL tFile;
    uint32_t iBytesWritten;
    char szDirFileName[64];

    //LOG_Send(LOG_MODULE, E_LOGSEV_DEBUG, "%s: %s", __func__, szFilename);

    if(iBufLen == 0)
        return ERR_SUCCESS;

    // Check if it's COM or PRO
    if(szFilename[3] == 'C')
    {
        sprintf(szDirFileName, "/%s/%s", SD_CARD_DIR_FW_DAT, szFilename);
    }
    else if(szFilename[3] == 'P')
    {
        sprintf(szDirFileName, "/%s/%s", SD_CARD_DIR_FW_PRO, szFilename);
    }
    else
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Bad filename %s", __func__, szFilename);
        return ERR_INVALID_ARG;
    }

    if(f_open(&tFile, szDirFileName, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
    {

        res = f_write(&tFile, pBuf, iBufLen, (void *)&iBytesWritten);

        if(res != FR_OK || iBytesWritten != iBufLen)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_write FAILED %i ", __func__, res);
            f_close(&tFile);
            return ERR_FATFS_WRITE;
        }
        res = f_sync(&tFile);
        if(res != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_sync FAILED %i ", __func__, res);
            f_close(&tFile);
            return ERR_FATFS_WRITE;
        }
        f_close(&tFile);

    }

    return ERR_SUCCESS;
}

#pragma GCC pop_options

FIL* SD_CARD_GetNewFileOpen(char *szFilename)
{
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s", __func__, szFilename);

    if(f_open(&SD_CARD_tFileRead, szFilename, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Can't create the file %s", __func__, szFilename);
        return NULL;
    }
    return &SD_CARD_tFileRead;
}

FIL* SD_CARD_GetFileOpen(char *szFilename)
{
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s", __func__, szFilename);

    if(f_open(&SD_CARD_tFileRead, szFilename, FA_READ) == FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: File opened", __func__);
        return &SD_CARD_tFileRead;
    }
    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Can't open the file", __func__);
    return NULL;
}

void SD_CARD_GetFileClose(FIL *pFile)
{
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);
    f_close(pFile);
}

uint32_t SD_CARD_GetFileSize(char *szFilename)
{
    FRESULT ret;
    FILINFO fno;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s", __func__, szFilename);

    ret = f_stat(szFilename, &fno);
    if(ret != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Can't open the file", __func__);
        return 0;
    }
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: Size %lu", __func__, fno.fsize);
    return fno.fsize;
}

E_Error SD_CARD_DeleteFile(char *szFilename)
{
    FRESULT ret;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s", __func__, szFilename);

    ret = f_unlink(szFilename);
    if(ret != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Can't delete the file", __func__);
        return ERR_OTHER;
    }
    return ERR_SUCCESS;
}

#pragma GCC push_options
#pragma GCC optimize ("O0")
E_Error SD_CARD_DeleteFirmware(char *szFilename)
{
    FRESULT res;
    DIR dir;
    static FILINFO fno;
    char *szPath;
    char szDirFileName[64];

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s", __func__, szFilename);

    // Check if it's COM or PRO or NVRAM
    if(szFilename[3] == 'C')
    {
        szPath = SD_CARD_DIR_FW_DAT;
    }
    else if(szFilename[3] == 'P')
    {
        szPath = SD_CARD_DIR_FW_PRO;
    }
    else
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Bad filename %s", __func__, szFilename);
        return ERR_INVALID_ARG;
    }

    res = f_opendir(&dir, szPath);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break; // break at the end of the file
            else {                                       /* It is a file. */
                if(snprintf(szDirFileName, sizeof(szDirFileName), "%s/%s", szPath, fno.fname) < 0)
                    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: szDirFileName truncated", __func__);
                res = f_unlink(szDirFileName);
                if(res != FR_OK)
                {
                    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Can't delete the file %s", __func__, szDirFileName);
                    return ERR_OTHER;
                }
            }
        }
        f_closedir(&dir);
    }
    return ERR_SUCCESS;
}
#pragma GCC pop_options

FRESULT SD_CARD_ScanAllFiles(char* path)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = SD_CARD_ScanAllFiles(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s/", __func__, path);
                path[i] = 0;
            } else {                                       /* It is a file. */
                LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: %s/%s [%li]", __func__, path, fno.fname, fno.fsize);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

static E_Error SD_CARD_GetFWName(char* szPath, char* szBuf)
{
    FRESULT res;
    DIR dir;
    static FILINFO fno;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    if(szBuf == NULL || szPath == NULL)
        return ERR_INVALID_ARG;

    res = f_opendir(&dir, szPath);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break; // break at the end of the file
			else if(fno.fname[0] == 'v' && fno.fname[1] == 'a' && fno.fname[2] == 'l')
			{
				// do nothing
			}
            else {                                       /* It is a file. */
                // return filename
                memcpy(szBuf, fno.fname, SD_CARD_FW_SIZE);
                szBuf[SD_CARD_FW_SIZE] = 0x00;
                f_closedir(&dir);
                return ERR_SUCCESS;
            }
        }
        f_closedir(&dir);
    }
    return ERR_FATFS_FILE_NOT_EXIST;
}

static bool SD_CARD_GetFWValid(char* szFilename)
{
    FIL tFile;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    if(f_open(&tFile, szFilename, FA_READ) == FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: File exist.", __func__);
        f_close(&tFile);
        return true;
    }
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: File does not exist.", __func__);
    return false;
}

#pragma GCC push_options
#pragma GCC optimize ("O0")
E_Error SD_CARD_SetFWValid(char *szFilename)
{
    FRESULT res = FR_OK;
    FIL tFile;
    uint8_t iBuf = '1';
    uint32_t iBytesWritten;
    char szDirFileName[64];

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    // Check if it's COM or PRO
    if(szFilename[3] == 'C')
    {
        sprintf(szDirFileName, "/%s/valid", SD_CARD_DIR_FW_DAT);
    }
    else if(szFilename[3] == 'P')
    {
        sprintf(szDirFileName, "/%s/valid", SD_CARD_DIR_FW_PRO);
    }
    else
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Bad filename %s", __func__, szFilename);
        return ERR_INVALID_ARG;
    }

    if(f_open(&tFile, szDirFileName, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: Can't create the file.", __func__);
        return ERR_FATFS_CREATE;
    }

    res = f_write(&tFile, &iBuf, 1, (void *)&iBytesWritten);
    if(res != FR_OK || iBytesWritten != 1)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_write FAILED %i ", __func__, res);
        f_close(&tFile);
        return ERR_FATFS_WRITE;
    }
    res = f_sync(&tFile);
    if(res != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_sync FAILED %i ", __func__, res);
        f_close(&tFile);
        return ERR_FATFS_WRITE;
    }

    f_close(&tFile);
    return ERR_SUCCESS;
}
#pragma GCC pop_options

E_Error SD_CARD_GetDATFWName(char* szBuf)
{
    char szPath[] = "//"SD_CARD_DIR_FW_DAT;

    return SD_CARD_GetFWName(szPath, szBuf);
}

bool SD_CARD_GetDATFWValid(void)
{
    char szFilename[] = "//"SD_CARD_DIR_FW_DAT"/valid";

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    return SD_CARD_GetFWValid(szFilename);
}

E_Error SD_CARD_GetPROFWName(char* szBuf)
{
    char szPath[] = "//"SD_CARD_DIR_FW_PRO;

    return SD_CARD_GetFWName(szPath, szBuf);
}

bool SD_CARD_GetPROFWValid(void)
{
    char szFilename[] = "//"SD_CARD_DIR_FW_PRO"/valid";

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    return SD_CARD_GetFWValid(szFilename);
}

uint32_t SD_CARD_GetFWSize(char *szPath)
{
    FRESULT res;
    DIR dir;
    static FILINFO fno;

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);

    res = f_opendir(&dir, szPath);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break; // break at the end of the file
            else if(fno.fname[0] == 'v' && fno.fname[1] == 'a' && fno.fname[2] == 'l')
			{
            	// do nothing
			}
            else /* It is a file. */
            {
                // return file size
                uint32_t iSize = fno.fsize;
                f_closedir(&dir);
                LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: Size %lu", __func__, iSize);
                return iSize;
            }
        }
        f_closedir(&dir);
    }
    return 0;
}

E_Error SD_CARD_GetFWBlock(uint32_t iBlockNumber, uint16_t iBlockSize, uint8_t *pBlock, char* szPath)
{
    FRESULT res;
    DIR dir;
    FILINFO fno;
    FIL file;
    char szDirFileName[64];
    UINT iReadSize;

    if(iBlockNumber%400 == 0)
        LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: Block %li", __func__, iBlockNumber);

    res = f_opendir(&dir, szPath);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break; // break at the end of the file
			else if(fno.fname[0] == 'v' && fno.fname[1] == 'a' && fno.fname[2] == 'l')
			{
				// do nothing
			}
            else {                                       /* It is a file. */
                // return block
                if(snprintf(szDirFileName, sizeof(szDirFileName), "%s/%s", szPath, fno.fname) < 0)
                    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: szDirFileName truncated", __func__);
                
        
                res = f_open(&file, szDirFileName, FA_READ);
        
                
                if (res != FR_OK)
                {
                    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_open failed %i", __func__, res);
                    f_closedir(&dir);
                    return ERR_FATFS_READ;
                }
                
        
                res = f_lseek(&file, iBlockNumber*iBlockSize);
        
                
                if (res != FR_OK)
                {
                    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_lseek failed %i", __func__, res);
                    f_close(&file);
                    f_closedir(&dir);
                    return ERR_FATFS_READ;
                }
                
        
                res = f_read(&file, pBlock, iBlockSize, &iReadSize);
        
                
                if (res != FR_OK)
                {
                    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_read failed %i", __func__, res);
                    f_close(&file);
                    f_closedir(&dir);
                    return ERR_FATFS_READ;
                }
        
                f_close(&file);
        
                f_closedir(&dir);
        

                return ERR_SUCCESS;
            }
        }

        f_closedir(&dir);

    }
    return ERR_FATFS_FILE_NOT_EXIST;
}


E_Error SD_CARD_IsPresent(void)
{
    if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET) return ERR_SD_CARD_NOT_PRESENT;

    return ERR_SUCCESS;
}
E_Error SD_CARD_IsWriteEnable(void)
{
    if(HAL_GPIO_ReadPin(SD_WRITE_ENABLE_GPIO_Port, SD_WRITE_ENABLE_Pin) != GPIO_PIN_RESET) return ERR_SD_CARD_WRITE_ENABLE;
    
    return ERR_SUCCESS;
}

E_Error SD_CARD_IsReady(void)
{
    E_Error err = SD_CARD_IsPresent();
    if (err != ERR_SUCCESS) return err;
    err = SD_CARD_IsWriteEnable();
    return err;
}

bool SD_CARD_IsBspInit(void)
{
    return bIsBspSdCardInit;
}

int8_t SD_CARD_BspInit(void)
{

	if (BSP_SD_Init() == MSD_OK)
    {
        GL_SD_CARD_iStatus = BSP_SD_GetCardState();
        if (GL_SD_CARD_iStatus == SD_TRANSFER_OK) 
        {
            bIsBspSdCardInit = true;
            return USBD_OK;
        }
    }
    return USBD_FAIL;
}


SD_CARD_State_e SD_CARD_GetState(void)
{
    return GL_SD_CARD_eState;
}

void SD_CARD_SetState(SD_CARD_State_e eState)
{
    GL_SD_CARD_eState = eState;
}


void SD_CARD_UnitTest(void)
{
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s", __func__);
    srand((unsigned int)time(NULL));

    // SD Card presence check
    bool bSDDetectState = (HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) == GPIO_PIN_RESET);
    bool bSDWriteEnableState = (HAL_GPIO_ReadPin(SD_WRITE_ENABLE_GPIO_Port, SD_WRITE_ENABLE_Pin) == GPIO_PIN_RESET);
    char sSDCardDetect[32], sWriteEnable[32];

    sprintf(sSDCardDetect, "%s", (bSDDetectState) ? "present" : "not present");
    sprintf(sWriteEnable, "%s", (bSDWriteEnableState) ? "unlock" : "lock");
    
    if (!bSDDetectState || !bSDWriteEnableState)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s SD Card presence check failed. SD Card is %s and %s", __func__, sSDCardDetect, sWriteEnable);
        return;
    }

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s SD Card presence check pass. SD Card is %s and %s", __func__, sSDCardDetect, sWriteEnable);
#if 0 // testing with unit initcard
    if(FATFS_LinkDriver(&SD_Driver, SD_CARD_szPath) != 0)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: FATFS_LinkDriver Failed", __func__);
            return ERR_FATFS_CREATE;
        }

    // Check mount FS
    if(f_mount(&SD_CARD_tFatFs, (TCHAR const*)SD_CARD_szPath, 1) != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: No FS, create one", __func__);
        if (f_mkfs(SD_CARD_szPath, FM_ANY, 0, GL_SD_CARD_workBuffer, sizeof(GL_SD_CARD_workBuffer)) != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: make file systeme failed.", __func__);
            return;
        }
        if(f_mount(&SD_CARD_tFatFs, (TCHAR const*)SD_CARD_szPath, 0) != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: mount failed.", __func__);
            return;
        }
    }

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s mount pass.", __func__);
#endif // testing with unit initcard
    // create folder
    if(SD_CARD_TestAndCreateFolder("UnitTest") != ERR_SUCCESS)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: create folder failed.", __func__);
        return;
    }

    // write
    int offset;
    UINT iBytesWritten;
    FRESULT res;
    uint32_t iFileCount = 5;
    uint32_t iRowCount = 10000;
    char szFileName[32];
    
    for (int i = 0; i < iFileCount ; i++)
    {
        //memset(szFileName, 0, sizeof(szFileName));
        snprintf(szFileName, sizeof(szFileName), "/UnitTest/UnitTest%d.txt", i);
        if(f_open(&GL_SD_CARD_tFile, szFileName, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: create %s failed.", __func__, szFileName);
            return;
        }
        //memset(GL_SD_CARD_szWorkBuffer, 0, WORK_BUFFER_SIZE);
        offset = snprintf(GL_SD_CARD_szWorkBuffer, WORK_BUFFER_SIZE , "%s\n", szFileName);

        offset += snprintf(GL_SD_CARD_szWorkBuffer + offset, WORK_BUFFER_SIZE  - offset, "*************************************************************\n");
        offset += snprintf(GL_SD_CARD_szWorkBuffer + offset, WORK_BUFFER_SIZE  - offset, " Time     SvO2   MPA   (dp/dt)max   Pressure   AP   Flow Rate\n");


        res = f_write(&GL_SD_CARD_tFile, GL_SD_CARD_szWorkBuffer, offset, (void *)&iBytesWritten);

        if(res != FR_OK || iBytesWritten != offset)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: write header for %s failed.", __func__, szFileName);
            f_close(&GL_SD_CARD_tFile);
            return;
        }


        for (uint32_t j = 0; j < iRowCount; j++)
        {
            //memset(GL_SD_CARD_szWorkBuffer, 0, WORK_BUFFER_SIZE);
            offset = snprintf(GL_SD_CARD_szWorkBuffer, WORK_BUFFER_SIZE , "%06lu     %02lu    %03lu      %03lu         %03lu      %02lu       %01lu\n",
                j,   // Time
                UTILS_GetRandomUint32(35,50),       // SvO2
                UTILS_GetRandomUint32(100,150),     // MPA
                UTILS_GetRandomUint32(130,200),     // (dp/dt)max
                UTILS_GetRandomUint32(120,170),     // Pressure
                UTILS_GetRandomUint32(60,99),       // AP
                UTILS_GetRandomUint32(2,7));        // Flow Rate
    
            res = f_write(&GL_SD_CARD_tFile, GL_SD_CARD_szWorkBuffer, offset, &iBytesWritten);
    
            if (res != FR_OK || iBytesWritten != offset)
            {
                LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: write data %i failed for %s.", __func__, j, szFileName);
                f_close(&GL_SD_CARD_tFile);
                return;
            }
        }

        f_sync(&GL_SD_CARD_tFile);
        if(res != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: f_sync for %s failed.", __func__, szFileName);
            f_close(&GL_SD_CARD_tFile);
            return;
        }
        f_close(&GL_SD_CARD_tFile);

    }

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s write pass.", __func__);
    
    // read
    uint32_t iBytesRead;
    for (int i = 0; i < iFileCount; i++)
    {
        sprintf(szFileName, "/UnitTest/UnitTest%i.txt", i);
        if(f_open(&GL_SD_CARD_tFile, szFileName, FA_READ) != FR_OK)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: open %s failed.", __func__, szFileName);
            return;
        }

        res = f_read(&GL_SD_CARD_tFile, GL_SD_CARD_szWorkBuffer, WORK_BUFFER_SIZE , (void *)&iBytesRead);

        if((iBytesRead <= 0) || (res != FR_OK))
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: read %s failed.", __func__, szFileName);
            f_close(&GL_SD_CARD_tFile);
            return;
        }
        f_close(&GL_SD_CARD_tFile);

    }

    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s read pass.", __func__);
    
    // scan folder UnitTest
    char buff[256];
    strcpy(buff, "/UnitTest");
    res = SD_CARD_ScanAllFiles(buff);
    if(res != FR_OK)
    {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: scan files failed.", __func__, res);
        return;
    }
    
    LOG_Send(LOG_MODULE, E_LOGSEV_INFO, "%s: Unit test pass.", __func__);

}

#endif

