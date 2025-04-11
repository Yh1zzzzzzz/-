#include "spi_lcd.h"
#include <stdio.h>
//#include "stdio.h"
float time = .0f;
char SPI_timer[8];
unsigned char LCD_addr[4][8]={
	{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},  		//第一行
	{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},		//第二行
	{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},		//第三行
	{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}		//第四行
	};
char *AD1674 = "AD1674:";
char *ADC = "ADC:";
unsigned char* AD1674_value_char;
unsigned char* ADC_value_char;
void spi_wirte_data(unsigned char data)
{
    unsigned char send[3];
    send[0] = 0xfa;
    send[1] = data & 0xf0;
    send[2] = (data << 4) & 0xf0;
    CS_H
    HAL_SPI_Transmit(&hspi1, send, 3, 1000);
    CS_L;
}
void spi_wirte_cmd(unsigned char cmd)
{
    unsigned char send[3];
    send[0] = 0xf8;
    send[1] = cmd & 0xf0;
    send[2] = (cmd << 4) & 0xf0;
    CS_H;
    HAL_Delay(1);
    HAL_SPI_Transmit(&hspi1, send, 3, 1000);
    CS_L;
}
void LCE_init(void){
	for(int i = 0;i < 8;i++){
		 SPI_timer[i] = '0';
	}
    HAL_Delay(50);
    spi_wirte_cmd(0x30);
    HAL_Delay(2);
    //spi_wirte_cmd(0x03);
    HAL_Delay(2);
    spi_wirte_cmd(0x0c);
		HAL_Delay(1);
		//spi_wirte_cmd(0x10);//Cursor Display Control光标设置
    HAL_Delay(2);
    spi_wirte_cmd(0x01);
    HAL_Delay(2);
    //spi_wirte_cmd(0x14);
}
void LCD_Display_Words(unsigned char x,unsigned char y,unsigned char*str)
{ 
	spi_wirte_cmd(LCD_addr[x][y]); //写初始光标位置
	while(*str != 0)
    { 
      spi_wirte_data(*str);    //写数据
      str++;     
    }
}
void FloatToUint8(uint8_t * char_array,float data)
{
    uint8_t i;
    for(i=0;i<4;i++)
    {
        char_array[i] = ((uint8_t*)(&data))[i];
    }
}
void LCD_Display_Lab(){
  LCD_Display_Words(0, 0, AD1674);

  FloatToUint8(AD1674_value_char, AD1674_value);
  LCD_Display_Words(1, 0, AD1674_value_char); //打印AD1674的值

  LCD_Display_Words(2, 0, ADC);
  FloatToUint8(ADC_value_char, ADC_Value_float);
  
  LCD_Display_Words(3, 0, ADC_value_char); //打印ADC的值
  
  //打印ADC的值

}