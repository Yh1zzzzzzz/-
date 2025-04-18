#pragma onces
#include "595driver.h"
#include "stm32f1xx_hal.h"

int time595 = 0;

 const unsigned char digitMapCA[10] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
}; //共阳

const unsigned char digitMapCC[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};//共阴

const unsigned char DIG_CS_CA[10] = {
    0x01, // 0
    0x02, // 1
    0x04, // 2
    0x08, // 3
    0x10, // 4
    0x20, // 5
    0x40, // 6
    0x80 // 7
};

const unsigned char DIG_CS_CA_INVERTED[8] = {
    0xFE, // 取反后的0
    0xFD, // 取反后的1
    0xFB, // 取反后的2
    0xF7, // 取反后的3
    0xEF, // 取反后的4
    0xDF, // 取反后的5
    0xBF, // 取反后的6
    0x7F  // 取反后的7
};
 int count[8] = {0,0,0,0,0,0,0,0};
 /*** *74HC595 发送一个字节 
*即往74HC595的DS引脚发送一个字节
*/
void HC595_Send_Byte(unsigned char byte)
{
	unsigned char i;
	for (i = 0; i < 8; i ++)  //一个字节8位，传输8次，一次一位，循环8次，刚好移完8位
	{
	  /****  步骤1：将数据传到DS引脚    ****/
		if (byte & 0x80)        //先传输高位，通过与运算判断第八是否为1
			HC595_Data_High()    //如果第八位是1，则与 595 DS连接的引脚输出高电平
		else                    //否则输出低电平
			HC595_Data_Low()
		
		/*** 步骤2：SHCP每产生一个上升沿，当前的bit就被送入移位寄存器 ***/
		HC595_SHCP_Low()  // SHCP拉低
		HC595_SHCP_High()  // SHCP拉高， SHCP产生上升沿
		
		byte <<= 1;		// 左移一位，将低位往高位移，通过	if (byte & 0x80)判断低位是否为1
	}
}
 
/**
 *74HC595输出锁存 使能 
**/
void HC595_CS(void) 
{
	/**  步骤3：STCP产生一个上升沿，移位寄存器的数据移入存储寄存器  **/
	HC595_STCP_Low()   // 将STCP拉低
	HAL_Delay(1);           // 适当延时
	HC595_STCP_High()  // 再将STCP拉高，STCP即可产生一个上升沿
	HAL_Delay(1);
}
 
/**
 *发送多个字节
 *便于级联时数据的发送
 *级联N级，就需要发送N个字节控制HC595
***/
void HC595_Send_Multi_Byte(unsigned char *data, unsigned short int len)
{
	unsigned char i;
	for (i = 0; i < len; i ++ ) // len 个字节
	{
		HC595_Send_Byte(data[i]);
	}
	
	HC595_CS(); //先把所有字节发送完，再使能输出
}
//发送命令先发位选（）DIG_CS_CA
// 再发 段选digitMapCA
// 写入字节为右边的 Q7 - Q0 左边 Q7---Q0
 void LAB_595_display(void){
	 			char cmd[2];
	 
	 // clear  before  write ?
	 
	 	 // clear  before  write ?
	 // clear  before  write ?
	 // clear  before  write ?

		for(int i = 0; i < 8; i++){
			cmd[0] = DIG_CS_CA_INVERTED[7-i];
			cmd[1] = digitMapCC[count[i]];
			HC595_Send_Multi_Byte(cmd, 2);	
			//HAL_Delay(1);
		
		}
 }