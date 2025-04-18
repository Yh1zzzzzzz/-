#pragma once
#include "stm32f1xx_hal.h"
extern SPI_HandleTypeDef hspi1;
extern char SPI_timer[8];

#define SPI_SW_SS_Pin GPIO_PIN_4
#define SPI_SW_SS_GPIO_Port GPIOC
#define CS_H HAL_GPIO_WritePin(SPI_SW_SS_GPIO_Port, SPI_SW_SS_Pin, 1);
#define CS_L HAL_GPIO_WritePin(SPI_SW_SS_GPIO_Port, SPI_SW_SS_Pin, 0);
void spi_wirte_data(unsigned char data);
void spi_wirte_cmd(unsigned char cmd);
void LCE_init(void);
void LCD_Display_Words(unsigned char x,unsigned char y,unsigned char*str);
#define history_key_size 16
void LAB2_request2(void);
extern ADC_value = 0;
extern ADC_Value_float = 0;