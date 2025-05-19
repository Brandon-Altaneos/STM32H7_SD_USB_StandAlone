/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v1.0_Cube
  * @brief          : Memory management layer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "globals.h"

#if defined(SD_DISKIO)
#include "sd_diskio.h"
#elif defined(USBDMSC)
#include "usbdmsc.h"
#else
#include "bsp_driver_sd.h"
#endif // USBDMSC


#define LOG_MODULE "USBD_STORAGE_IF"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_HS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_HS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_HS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_HS(uint8_t lun);
static int8_t STORAGE_GetCapacity_HS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_HS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_HS(uint8_t lun);
static int8_t STORAGE_Read_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_HS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_HS =
{
  STORAGE_Init_HS,
  STORAGE_GetCapacity_HS,
  STORAGE_IsReady_HS,
  STORAGE_IsWriteProtected_HS,
  STORAGE_Read_HS,
  STORAGE_Write_HS,
  STORAGE_GetMaxLun_HS,
  (int8_t *)STORAGE_Inquirydata_HS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the storage unit (medium).
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_HS(uint8_t lun)
{
  /* USER CODE BEGIN 9 */
  return USBDMSC_Init_HS(lun);
  /* USER CODE END 9 */
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number.
  * @param  block_num: Number of total block number.
  * @param  block_size: Block size.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_HS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 10 */
  return USBDMSC_GetCapacity_HS(lun, block_num, block_size);
  /* USER CODE END 10 */
}

/**
  * @brief   Checks whether the medium is ready.
  * @param  lun:  Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_HS(uint8_t lun)
{
  /* USER CODE BEGIN 11 */
#if defined(USBDMSC)
  return USBDMSC_IsReady_HS(lun);
#else
  static int8_t prev_status = 0;
  USBD_StatusTypeDef ret = USBD_FAIL;

  if (BSP_SD_IsDetected() != SD_NOT_PRESENT)
  {
    if (prev_status < 0)
    {
      //BSP_SD_Init();
      prev_status = 0;

    }

    if (BSP_SD_GetCardState() == SD_TRANSFER_OK)
    {
      ret = USBD_OK;
    }
  }
  else if (prev_status == 0)
  {
    prev_status = -1;
  }
  return ret;
#endif // USBDMSC
  /* USER CODE END 11 */
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_HS(uint8_t lun)
{
  /* USER CODE BEGIN 12 */
  return USBDMSC_IsWriteProtected_HS(lun);
  /* USER CODE END 12 */
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number.
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 13 */
#if defined(SD_DISKIO)

  DRESULT res = SD_Driver.disk_read(lun, buf, blk_addr, blk_len);
  if (res != RES_OK)
  {
    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: disk_read [blk_addr %i; blk_len %i] Failed (%i)", __func__, blk_addr, blk_len, res);
    return USBD_FAIL;
  }

  return USBD_OK;
#elif defined(USBDMSC)
  return USBDMSC_Read_HS(lun, buf, blk_addr, blk_len);
#else
  USBD_StatusTypeDef ret = USBD_FAIL;
  uint32_t timeout = 100000;
  if (BSP_SD_IsDetected() != SD_NOT_PRESENT)
  {
	  if (BSP_SD_ReadBlocks((uint32_t *) buf, blk_addr, blk_len, 4000) != MSD_OK)
    {
      LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: BSP_SD_ReadBlocks Failed (blk_addr %i; blk_len %i)", __func__, blk_addr, blk_len);
      ret
    }

    /* Wait until SD card is ready to use for new operation */
    while (BSP_SD_GetCardState() != SD_TRANSFER_OK)
    {
      if (timeout-- == 0)
      {
        LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: BSP_SD_GetCardState timeout", __func__, blk_addr, blk_len);
        return ret;
      }
    }

    ret = USBD_OK;
  }
  return ret;
#endif // USBDMSC
  /* USER CODE END 13 */
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number.
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 14 */
#if defined(SD_DISKIO)

  DRESULT res = SD_Driver.disk_write(lun, buf, blk_addr, blk_len);
  if (res != RES_OK)
  {
    LOG_Send(LOG_MODULE, E_LOGSEV_ERROR, "%s: disk_write [blk_addr %i; blk_len %i] Failed (%i)", __func__, blk_addr, blk_len, res);
    return USBD_FAIL;
  }
  return USBD_OK;
#elif defined(USBDMSC)
  return USBDMSC_Write_HS(lun, buf, blk_addr, blk_len);
#else
  USBD_StatusTypeDef ret = USBD_FAIL;
  uint32_t timeout = 100000;

  if (BSP_SD_IsDetected() != SD_NOT_PRESENT)
  {
    BSP_SD_WriteBlocks((uint32_t *) buf, blk_addr, blk_len, 10000);

    /* Wait until SD card is ready to use for new operation */
    while (BSP_SD_GetCardState() != SD_TRANSFER_OK)
    {
      if (timeout-- == 0)
      {
        return ret;
      }
    }

    ret = USBD_OK;
  }
  return ret;
#endif // USBDMSC
  /* USER CODE END 14 */
}

/**
  * @brief  Returns the Max Supported LUNs.
  * @param  None
  * @retval Lun(s) number.
  */
int8_t STORAGE_GetMaxLun_HS(void)
{
  /* USER CODE BEGIN 15 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 15 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

