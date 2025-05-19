/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : fatfs_platform.c
  * @brief          : fatfs_platform source file
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
#include "globals.h"

#define LOG_MODULE  "FATFS_PLATFORM"
/* USER CODE END Header */
#include "fatfs_platform.h"

uint8_t	BSP_PlatformIsDetected(void) {
    uint8_t status = SD_PRESENT;
    /* Check SD card detect pin */
    if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET)
    {
        status = SD_NOT_PRESENT;
    }
    /* USER CODE BEGIN 1 */
    // char sSDCardDetect[32], sWriteEnable[32];
    // sprintf(sSDCardDetect, "%s", (status == SD_PRESENT) ? "present" : "not present");
    // sprintf(sWriteEnable, "%s", (HAL_GPIO_ReadPin(SD_WRITE_ENABLE_GPIO_Port, SD_WRITE_ENABLE_Pin) == GPIO_PIN_RESET) ? "unlock" : "lock");
    // LOG_Send(LOG_MODULE, E_LOGSEV_DEBUG, "%s: SD Card is %s and %s", __func__, sSDCardDetect, sWriteEnable);
    /* USER CODE END 1 */
    return status;
}
