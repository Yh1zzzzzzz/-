#include "sw_lcd.h"
#include "stm32f1xx_hal.h"
unsigned char LCD_addr_SW[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  		//第一行
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//第二行
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//第三行
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//第四行
	};
void Write_Byte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
        HAL_GPIO_WritePin(SW_SDA_GPIO_Port, SW_SDA_Pin, dat & 0x80);
        SCK_L;
        HAL_Delay(1);
        SCK_H;
        dat<<=1;
	}
}
void Write_Cmd(unsigned char dat)
{
    CS_H_SW;
    HAL_Delay(1);
    Write_Byte(0xf8);
    Write_Byte(dat & 0xf0);
    Write_Byte((dat << 4) & 0xf0);
    CS_L_SW;
}
void Write_Data(unsigned char dat)
{
    CS_H_SW;
    HAL_Delay(1);
    Write_Byte(0xfa);
    Write_Byte(dat & 0xf0);
    Write_Byte((dat << 4) & 0xf0);
    CS_L_SW;
}
void LCD_Init(void)
{
    HAL_Delay(50);
    Write_Cmd(0x30);
    HAL_Delay(2);
    Write_Cmd(0x03);
    HAL_Delay(2);
    Write_Cmd(0x0c);
    HAL_Delay(2);
    Write_Cmd(0x01);
    HAL_Delay(2);
    Write_Cmd(0x06);
}

void LCD_Display_Words_SW(unsigned char x,unsigned char y,unsigned char*str)
{ 
	Write_Cmd(LCD_addr_SW[x][y]); //写初始光标位置
	while(*str>0)
    { 
      Write_Data(*str);    //写数据
      str++;     
    }
}
void Write_CQU(void){
//中文位置打印学号
    LCD_Display_Words_SW(0,1,"重庆大学");
}
void Write_automation(void){
//中文位置
    LCD_Display_Words_SW(2,0,"自动化学院");
}

void Write_xuehao(void){
//中文位置打印
    LCD_Display_Words_SW(3,0,"二零二二一六八九");
}
void Write_xuehao_eng(void){
//英语位置打印学号
    LCD_Display_Words_SW(4,2,"2021689");
}
void LCD_Display_SW(void){
    Write_CQU();
    Write_automation();
    Write_xuehao();
    Write_xuehao_eng();
}
