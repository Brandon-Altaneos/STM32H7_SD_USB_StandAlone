/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "globals.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USB_HS_DIR_Pin GPIO_PIN_11
#define USB_HS_DIR_GPIO_Port GPIOI
#define USB_HS_STP_Pin GPIO_PIN_0
#define USB_HS_STP_GPIO_Port GPIOC
#define USB_HS_NXT_Pin GPIO_PIN_3
#define USB_HS_NXT_GPIO_Port GPIOC
#define USB_HS_D0_Pin GPIO_PIN_3
#define USB_HS_D0_GPIO_Port GPIOA
#define nRST_USB_Pin GPIO_PIN_4
#define nRST_USB_GPIO_Port GPIOA
#define USB_HS_CK_Pin GPIO_PIN_5
#define USB_HS_CK_GPIO_Port GPIOA
#define USB_HS_D1_Pin GPIO_PIN_0
#define USB_HS_D1_GPIO_Port GPIOB
#define USB_HS_D2_Pin GPIO_PIN_1
#define USB_HS_D2_GPIO_Port GPIOB
#define USB_HS_D3_Pin GPIO_PIN_10
#define USB_HS_D3_GPIO_Port GPIOB
#define USB_HS_D4_Pin GPIO_PIN_11
#define USB_HS_D4_GPIO_Port GPIOB
#define USB_HS_D5_Pin GPIO_PIN_12
#define USB_HS_D5_GPIO_Port GPIOB
#define USB_HS_D6_Pin GPIO_PIN_13
#define USB_HS_D6_GPIO_Port GPIOB
#define DEBUG_TX_Pin GPIO_PIN_14
#define DEBUG_TX_GPIO_Port GPIOB
#define DEBUG_RX_Pin GPIO_PIN_15
#define DEBUG_RX_GPIO_Port GPIOB
#define SD_CARD_DETECT_Pin GPIO_PIN_3
#define SD_CARD_DETECT_GPIO_Port GPIOD
#define SD_WRITE_ENABLE_Pin GPIO_PIN_4
#define SD_WRITE_ENABLE_GPIO_Port GPIOD
#define USB_HS_D7_Pin GPIO_PIN_5
#define USB_HS_D7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
