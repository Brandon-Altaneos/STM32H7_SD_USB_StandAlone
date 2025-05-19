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
/*==                              usbdmsc.c                              ==*/
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

/*
** Include section
*/

#include "usbdmsc.h"
#include "bsp_driver_sd.h"
#include "ff_gen_drv.h"
#include "usbd_def.h"
#include "sd_card.h"
#include "sd_diskio.h"
/*
** Macro Definitions
*/
#define LOG_MODULE "USBDMSC"
#undef USBDMSC_READ_WRITE_IMP

#define STORAGE_LUN_NBR     1

#define USBDMC_TIMEOUT          100000
#define SD_TIMEOUT              (5 * 1000)
#define SD_DEFAULT_BLOCK_SIZE   512

/*
** Type Definitions
*/

/*
** Exported Global Data
*/

/*
** File Data
*/

// static uint8_t GL_USBDMSC_aScratch[BLOCKSIZE];
static volatile uint8_t GL_USBDMSC_iStat = STA_NOINIT;
static volatile uint8_t GL_USBDMSC_iWriteStatus = 0, GL_USBDMSC_iReadStatus = 0;
RAM_D1_NOCACHE uint8_t GL_USBDMSC_buf[SD_DEFAULT_BLOCK_SIZE] = {0};
/*
** Local Function Prototypes
*/
#ifdef USBDMSC_READ_WRITE_IMP
static int8_t USBMSC_CheckStatusWithTimeout(uint32_t iTimeout);
#endif // USBDMSC_READ_WRITE_IMP
// static int8_t USBMSC_CheckStatus(void); // check with USB

/*
** Function definitions
*/

int8_t USBDMSC_Init_HS(uint8_t iLun)
{
    UNUSED(iLun);

    if (SD_CARD_GetState() == SD_CARD_STATE_LOCK_FATFS)
        return USBD_OK;

    SD_CARD_SetState(SD_CARD_STATE_LOCK_USBMSC);

    // if (!SD_CARD_IsBspInit())
    // {
    //     return SD_CARD_BspInit();
    // }

    return USBD_OK;
}

int8_t USBDMSC_GetCapacity_HS(uint8_t iLun, uint32_t *iBlock_num, uint16_t *iBlock_size)
{
    int8_t iRet = USBD_FAIL;

    UNUSED(iLun);

    if (SD_CARD_GetState() == SD_CARD_STATE_LOCK_USBMSC)
    {
        HAL_SD_CardInfoTypeDef tInfo;
        E_Error err = SD_CARD_IsReady();
        if (err != ERR_SUCCESS)
        {
            LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: %s", __func__, err);
            return iRet;
        }
        BSP_SD_GetCardInfo(&tInfo);
        *iBlock_num =  tInfo.LogBlockNbr  - 1;
        *iBlock_size = tInfo.LogBlockSize;
        iRet = USBD_OK;
    }
    else
    {
        *iBlock_num = 0;
		*iBlock_size = 0;
		iRet =  USBD_FAIL;
    }

    return iRet;
}

int8_t USBDMSC_IsReady_HS(uint8_t iLun)
{
    UNUSED(iLun);

    if (SD_CARD_GetState() != SD_CARD_STATE_LOCK_USBMSC)
    {
        return USBD_FAIL;
    }

    E_Error err = SD_CARD_IsReady();
    if (err != ERR_SUCCESS)
    {
        return USBD_FAIL;
    }

    // if (!SD_CARD_IsBspInit())
    // {
    //     if (SD_CARD_BspInit() != USBD_OK)
    //     {
    //         return USBD_FAIL;
    //     }
    // }

    if (BSP_SD_GetCardState() != SD_TRANSFER_OK)
    {
        return USBD_BUSY;
    }

    return USBD_OK;
}

int8_t USBDMSC_IsWriteProtected_HS(uint8_t iLun)
{
    UNUSED(iLun);
    
    if (SD_CARD_IsWriteEnable() != ERR_SUCCESS) return USBD_FAIL;
    return USBD_OK;
}

int8_t USBDMSC_Read_HS(uint8_t iLun, uint8_t *pBuf, uint32_t iBlk_addr, uint16_t iBlk_len)
{
    if (SD_CARD_GetState() == SD_CARD_STATE_LOCK_USBMSC)
    {
#ifdef USBDMSC_READ_WRITE_IMP
        int8_t iRet = USBD_FAIL;
        uint32_t iTimeout = USBDMC_TIMEOUT;
        int iCount = 0;

        memset(GL_USBDMSC_buf, 0, SD_DEFAULT_BLOCK_SIZE);

        if (USBMSC_CheckStatusWithTimeout(SD_TIMEOUT) < 0) return iRet;

        for (int i = 0; i < iBlk_len; i++)
        {
            if (BSP_SD_ReadBlocks((uint32_t*)GL_USBDMSC_buf,
                                    (uint32_t) (iBlk_addr),
                                    1,
                                    SD_TIMEOUT) == MSD_OK)
            {
                while (BSP_SD_GetCardState() != SD_TRANSFER_OK)
                {
                    if (iTimeout-- == 0)
                    {  return iRet; }
                    iCount++;
                }
                memcpy(pBuf, GL_USBDMSC_buf, SD_DEFAULT_BLOCK_SIZE);
                iRet = USBD_OK;
            }
        }

        return iRet;
#else
        DRESULT res = SD_Driver.disk_read(iLun, pBuf, iBlk_addr, iBlk_len);
        if (res != RES_OK)
        {
          //LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: disk_read [blk_addr %i; blk_len %i] Failed (%i)", __func__, blk_addr, blk_len, res);
          return USBD_FAIL;
        }

        return USBD_OK;
#endif // USBDMSC_READ_WRITE_IMP
    }
    else
    {
        return USBD_FAIL;
    }
}

int8_t USBDMSC_Write_HS(uint8_t iLun, const uint8_t *pBuf, uint32_t iBlk_addr, uint16_t iBlk_len)
{
    if (SD_CARD_GetState() == SD_CARD_STATE_LOCK_USBMSC)
    {
#ifdef USBDMSC_READ_WRITE_IMP
    int8_t iRet = USBD_FAIL;
    uint32_t iTimeout = USBDMC_TIMEOUT;
    int iCount = 0;
    
    memcpy(GL_USBDMSC_buf, pBuf, SD_DEFAULT_BLOCK_SIZE);

    if (USBMSC_CheckStatusWithTimeout(SD_TIMEOUT) < 0) return iRet;

    for (int i = 0; i < iBlk_len; i++)
    {
        if(BSP_SD_WriteBlocks((uint32_t*)GL_USBDMSC_buf,
                                (uint32_t)(iBlk_addr),
                                1,
                                SD_TIMEOUT) == MSD_OK)
        {
            while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
            {
                if (iTimeout-- == 0)
                {  return iRet; }
                iCount++;
            }
            iRet = USBD_OK;
        }
    }
    return iRet;
#else
    DRESULT res = SD_Driver.disk_write(iLun, pBuf, iBlk_addr, iBlk_len);
    if (res != RES_OK)
    {
      //LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: disk_write [blk_addr %i; blk_len %i] Failed (%i)", __func__, blk_addr, blk_len, res);
      return USBD_FAIL;
    }
    return USBD_OK;
#endif // USBDMSC_READ_WRITE_IMP
    }
    else
    {

        return USBD_FAIL;
    }
}

int8_t USBDMSC_GetMaxiLun_HS(void)
{
    return (STORAGE_LUN_NBR - 1);
}

// callback

void USBMSC_ConnectCallback(void)
{

}

void USBMSC_DisconnectCallback(void)
{

}

// Callback bsp_driver_sd DMA

void USBDMSC_BSP_SD_WriteCpltCallback(void)
{
  GL_USBDMSC_iWriteStatus = 1;
}

void USBDMSC_BSP_SD_ReadCpltCallback(void)
{
  GL_USBDMSC_iReadStatus = 1;
}

// Local Functions
#ifdef USBDMSC_READ_WRITE_IMP
static int8_t USBMSC_CheckStatusWithTimeout(uint32_t iTimeout)
{
    uint32_t iTimer = HAL_GetTick();
    uint32_t iCount = 0;
    while (HAL_GetTick() - iTimer < iTimeout)
    {
        iCount++;
        if (BSP_SD_GetCardState() == SD_TRANSFER_OK) return 0;
    }

    return -1;
}
#endif // USBDMSC_READ_WRITE_IMP

/* check with USB
static int8_t USBMSC_CheckStatus(void)
{
  GL_USBDMSC_iStat = STA_NOINIT;

  if(BSP_SD_GetCardState() == MSD_OK)
  {
    GL_USBDMSC_iStat &= ~STA_NOINIT;
  }

  return GL_USBDMSC_iStat;
}
*/
