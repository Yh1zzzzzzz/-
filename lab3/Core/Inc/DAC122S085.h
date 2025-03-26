#ifndef __DAC122S085_H
#define __DAC122S085_H

#include "stm32f4xx_hal.h"

#define SPI_SW_SS_Pin GPIO_PIN_4
#define SPI_SW_SS_GPIO_Port GPIOC
#define CS_H HAL_GPIO_WritePin(SPI_SW_SS_GPIO_Port, SPI_SW_SS_Pin, 1);
#define CS_L HAL_GPIO_WritePin(SPI_SW_SS_GPIO_Port, SPI_SW_SS_Pin, 0);

// DAC通道定义
#define DAC_CHANNEL_A 0x00
#define DAC_CHANNEL_B 0x01

// 函数声明
void DAC122S085_Init(SPI_HandleTypeDef *hspi);
void DAC122S085_SetVoltage(SPI_HandleTypeDef *hspi, uint8_t channel, uint16_t value);
void DAC122S085(unsigned char ch, unsigned char md, unsigned int daout);
#endif /* __DAC122S085_H */