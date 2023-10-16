/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define HR_OE_Pin GPIO_PIN_10
#define HR_OE_GPIO_Port GPIOB
#define SPO2_OE_Pin GPIO_PIN_11
#define SPO2_OE_GPIO_Port GPIOB
#define RG_CLEAR_Pin GPIO_PIN_12
#define RG_CLEAR_GPIO_Port GPIOB
#define RG_CLK_Pin GPIO_PIN_13
#define RG_CLK_GPIO_Port GPIOB
#define RG_STROBE_Pin GPIO_PIN_14
#define RG_STROBE_GPIO_Port GPIOB
#define RG_DATA0_Pin GPIO_PIN_15
#define RG_DATA0_GPIO_Port GPIOB
#define BAR_R_OE_Pin GPIO_PIN_8
#define BAR_R_OE_GPIO_Port GPIOA
#define BAR_L_OE_Pin GPIO_PIN_9
#define BAR_L_OE_GPIO_Port GPIOA
#define MIXLED_OE_Pin GPIO_PIN_10
#define MIXLED_OE_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
