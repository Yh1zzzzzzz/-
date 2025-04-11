#ifndef __AD1674_H
#define __AD1674_H
#include "stm32f1xx_hal.h"
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

extern float AD1674_value;
void AD1674_Init(void);
uint16_t AD1674_ReadADC(void);
void AD1674_StartConversion(void);
uint8_t AD1674_IsConversionDone(void);
uint16_t AD1674_ReadValue(void);
#endif