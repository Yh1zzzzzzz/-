#ifndef __DAC0832_H
#define __DAC0832_H

#include "main.h"

// DAC0832引脚定义
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

// 函数声明
void DAC0832_Init(void);
void DAC0832_Write(uint8_t data);
void DAC0832_SineWave_Init(void);
void DAC0832_SineWave_Update(void);

#endif /* __DAC0832_H */
