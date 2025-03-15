
#define SW_CS_Pin GPIO_PIN_3
#define SW_CS_GPIO_Port GPIOB
#define SW_SDA_Pin GPIO_PIN_4
#define SW_SDA_GPIO_Port GPIOB
#define SW_SCK_Pin GPIO_PIN_5
#define SW_SCK_GPIO_Port GPIOB
#define CS_H_SW HAL_GPIO_WritePin(SW_CS_GPIO_Port, SW_CS_Pin, 1);
#define CS_L_SW HAL_GPIO_WritePin(SW_CS_GPIO_Port, SW_CS_Pin, 0);
#define SCK_H HAL_GPIO_WritePin(SW_SCK_GPIO_Port, SW_SCK_Pin, 1);
#define SCK_L HAL_GPIO_WritePin(SW_SCK_GPIO_Port, SW_SCK_Pin, 0);
#ifndef _SERIAL12864_H
#define _SERIAL12864_H
#define uchar unsigned char
#include "main.h"

#define LCD12864_CS_Pin GPIO_PIN_3
#define LCD12864_CS_GPIO_Port GPIOB
#define LCD12864_SDA_Pin GPIO_PIN_4
#define LCD12864_SDA_GPIO_Port GPIOB
#define LCD12864_SCK_Pin GPIO_PIN_5
#define LCD12864_SCK_GPIO_Port GPIOB

#define u8 unsigned char
#define u16 unsigned short


void delayMicroseconds(uint32_t us);
void SerialWriteData(uchar send);
void SerialWriteCmd(uchar send);
void InitLcd(void);
void DisplayAt(uchar hang,uchar lie,uchar *p);
//void Dis(const uchar *p);
void ChangeCR(uchar hang,uchar lie);
void ShowNumber(uchar high,uchar low);
void ShowNumber_16(u8 hang,u8 lie,u16 dat);
void SerialWriteData2(uchar dat);
void SerialWriteCmd2(uchar dat);
 //void delay_us(u16 us);
void DisplayAt2(uchar hang,uchar lie,uchar *p);
//void delay_ms(u16 ms);  //??????????????????
void	drawon(void);
void	drawoff(void);
void SerialShow_picture3232(u8 line,u8 column,u8 *address);
void SerialDraw_Clr(void);
void SerialShow_picture12864(u8 line,u8 column,u8 *address);
void Serial_draw_word(uchar *x, uchar y); 
void Serial_print_sinx(void);
void LCD_Display_SW(void);

#endif
