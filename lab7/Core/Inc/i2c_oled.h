#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f1xx_hal.h"
#include "main.h"
#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void  OLED_Disp_Init(void);
extern uint8_t  disp_buf[];	//��ʾ������
 extern uint8_t  code[];	//ASCLL��
extern int inter_cnt;
	void OLED_LAB_DISP_name(void);
		void OLED_LAB_DISP(void);
void s2_pressed(void);
void s3_pressed(void);
extern int Moter_direction;
extern int PWM_pulse;
extern volatile float RPM;
 void OLED_ShowNumber_16(unsigned char x,unsigned char y,unsigned short dat);
#endif
