#ifndef __TM1650_H
#define __TM1650_H

#include "main.h"

// TM1650命令地址
#define TM1650_DISPLAY_MODE_ADDR   0x48  // 显示模式设置地址
#define TM1650_KEY_SCAN_ADDR       0x49  // 键盘扫描地址
#define TM1650_DISPLAY_REG1_ADDR   0x68  // 第1位数码管
#define TM1650_DISPLAY_REG2_ADDR   0x6A  // 第2位数码管
#define TM1650_DISPLAY_REG3_ADDR   0x6C  // 第3位数码管
#define TM1650_DISPLAY_REG4_ADDR   0x6E  // 第4位数码管

// TM1650显示控制命令
#define TM1650_DISPLAY_ON          0x01  // 显示开
#define TM1650_DISPLAY_OFF         0x00  // 显示关
#define TM1650_KEYBOARD_ON         0x02  // 键盘扫描开
#define TM1650_NORMAL_MODE         0x04  // 普通模式
#define TM1650_BRIGHTNESS_MASK     0x70  // 亮度掩码

// 数码管段码表 (0-9, A-F)
extern const uint8_t TM1650_SEGMENT_MAP[16];

// 函数声明
void TM1650_Init(void);
void TM1650_SetBrightness(uint8_t brightness);
void TM1650_DisplayDigit(uint8_t position, uint8_t digit, uint8_t dot);
void TM1650_DisplayNumber(uint16_t number);
uint8_t TM1650_ReadKey(void);
void TM1650_DisplayKeyValue(uint8_t keyValue);

#define SW_CS_Pin GPIO_PIN_3
#define SW_CS_GPIO_Port GPIOB
#define SW_SDA_Pin GPIO_PIN_4
#define SW_SDA_GPIO_Port GPIOB
#define SW_SCK_Pin GPIO_PIN_5
#define SW_SCK_GPIO_Port GPIOB
 
#define TM1650_SCL_1 HAL_GPIO_WritePin(SW_SCK_GPIO_Port, SW_SCK_Pin, 1)
#define TM1650_SCL_0 HAL_GPIO_WritePin(SW_SCK_GPIO_Port, SW_SCK_Pin, 0) 
 
#define TM1650_SDA_1 HAL_GPIO_WritePin(SW_SDA_GPIO_Port, SW_SDA_Pin, 1)
#define TM1650_SDA_0 HAL_GPIO_WritePin(SW_SDA_GPIO_Port, SW_SDA_Pin, 0) 

#define history_key_size 16
extern char curr_key;
extern char history_key[history_key_size];
extern int index_key;

#endif /* __TM1650_H */