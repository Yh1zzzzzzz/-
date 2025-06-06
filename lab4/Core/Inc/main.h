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
#define A_38_Pin GPIO_PIN_1
#define A_38_GPIO_Port GPIOC
#define B_38_Pin GPIO_PIN_2
#define B_38_GPIO_Port GPIOC
#define C_38_Pin GPIO_PIN_3
#define C_38_GPIO_Port GPIOC
#define SPI_SW_SS_Pin GPIO_PIN_4
#define SPI_SW_SS_GPIO_Port GPIOC
#define AD1674_CS_Pin GPIO_PIN_11
#define AD1674_CS_GPIO_Port GPIOB
#define DB0_Pin GPIO_PIN_12
#define DB0_GPIO_Port GPIOB
#define DB1_Pin GPIO_PIN_13
#define DB1_GPIO_Port GPIOB
#define DB2_Pin GPIO_PIN_14
#define DB2_GPIO_Port GPIOB
#define DB3_Pin GPIO_PIN_15
#define DB3_GPIO_Port GPIOB
#define DB4_Pin GPIO_PIN_6
#define DB4_GPIO_Port GPIOC
#define DB5_Pin GPIO_PIN_7
#define DB5_GPIO_Port GPIOC
#define DB6_Pin GPIO_PIN_8
#define DB6_GPIO_Port GPIOC
#define DB7_Pin GPIO_PIN_9
#define DB7_GPIO_Port GPIOC
#define STS_Pin GPIO_PIN_8
#define STS_GPIO_Port GPIOA
#define A0_Pin GPIO_PIN_9
#define A0_GPIO_Port GPIOA
#define RC_Pin GPIO_PIN_10
#define RC_GPIO_Port GPIOA
#define CE_Pin GPIO_PIN_11
#define CE_GPIO_Port GPIOA
#define I2_8_Pin GPIO_PIN_12
#define I2_8_GPIO_Port GPIOA
#define SER_595_Pin GPIO_PIN_15
#define SER_595_GPIO_Port GPIOA
#define RCK_595_Pin GPIO_PIN_10
#define RCK_595_GPIO_Port GPIOC
#define SCK_595_Pin GPIO_PIN_11
#define SCK_595_GPIO_Port GPIOC
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
