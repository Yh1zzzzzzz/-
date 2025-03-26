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
#include "stm32f1xx_hal.h"

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
#define LED_D1_Pin GPIO_PIN_13
#define LED_D1_GPIO_Port GPIOC
#define SPI_SW_SS_Pin GPIO_PIN_4
#define SPI_SW_SS_GPIO_Port GPIOC
#define DAC_D0_Pin GPIO_PIN_12
#define DAC_D0_GPIO_Port GPIOB
#define DAC_D1_Pin GPIO_PIN_13
#define DAC_D1_GPIO_Port GPIOB
#define DAC_D2_Pin GPIO_PIN_14
#define DAC_D2_GPIO_Port GPIOB
#define DAC_D3_Pin GPIO_PIN_15
#define DAC_D3_GPIO_Port GPIOB
#define DAC_D4_Pin GPIO_PIN_6
#define DAC_D4_GPIO_Port GPIOC
#define DAC_D5_Pin GPIO_PIN_7
#define DAC_D5_GPIO_Port GPIOC
#define DAC_D6_Pin GPIO_PIN_8
#define DAC_D6_GPIO_Port GPIOC
#define DAC_D7_Pin GPIO_PIN_9
#define DAC_D7_GPIO_Port GPIOC
#define DAC_CS_Pin GPIO_PIN_8
#define DAC_CS_GPIO_Port GPIOA
#define DAC_RW_Pin GPIO_PIN_9
#define DAC_RW_GPIO_Port GPIOA
#define SW_CS_Pin GPIO_PIN_3
#define SW_CS_GPIO_Port GPIOB
#define SW_SDA_Pin GPIO_PIN_4
#define SW_SDA_GPIO_Port GPIOB
#define SW_SCK_Pin GPIO_PIN_5
#define SW_SCK_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
