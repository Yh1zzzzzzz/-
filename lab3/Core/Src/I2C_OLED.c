/************************************************************************************

* 
* 
* Description:128*64?????OLED????????????????????SD1306????IIC??????????
*
*???????????STM32F103RCT6??I2C2  ??PB10(SCL),PB11(SDA)
*                                   ???? STM32???J2??PB10??CSB/JGCJ_1???CN1??SCL,
*                                        STM32???J2??PB11??CSB/JGCJ_1???CN1??SDA,

Function List:
*	1. void I2C_Configuration(void) -- ????CPU?????I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- ?????????§Õ???byte??????
* 3. void WriteCmd(unsigned char I2C_Command) -- §Õ????
* 4. void WriteDat(unsigned char I2C_Data) -- §Õ????
* 5. void OLED_Init(void) -- OLED???????
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ?????????????
* 7. void OLED_Fill(unsigned char fill_Data) -- ??????
* 8. void OLED_CLS(void) -- ????
* 9. void OLED_ON(void) -- ????
* 10. void OLED_OFF(void) -- ???
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ????????(?????§³??6*8??8*16????)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ???????(??????????????????codetab.h??)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP??
*
* History: none;
*
*************************************************************************************/

#include "i2c_oled.h"
//#include "delay.h"
#include "codetabs.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
 uint8_t  disp_buf[]="1234";	//?????????
 uint8_t  code[]="0123456789";	//ASCLL??
 int inter_cnt;
//static void MX_I2C2_Init(void)
//{

//  /* USER CODE BEGIN I2C2_Init 0 */

//  /* USER CODE END I2C2_Init 0 */

//  /* USER CODE BEGIN I2C2_Init 1 */

//  /* USER CODE END I2C2_Init 1 */
//  hi2c1.Instance = I2C2;
//  hi2c1.Init.ClockSpeed = 100000;
//  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//    Error_Handler();
//  }


//void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hi2c->Instance==I2C2)
//  {
//  /* USER CODE BEGIN I2C2_MspInit 0 */

//  /* USER CODE END I2C2_MspInit 0 */

//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**I2C2 GPIO Configuration
//    PB10     ------> I2C2_SCL
//    PB11     ------> I2C2_SDA
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    /* Peripheral clock enable */
//    __HAL_RCC_I2C2_CLK_ENABLE();
//  /* USER CODE BEGIN I2C2_MspInit 1 */

//  /* USER CODE END I2C2_MspInit 1 */
//  }


//void I2C_Configuration(void)
//{
//	I2C_InitTypeDef  I2C_InitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure; 

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

//	/*STM32F103C8T6§à??????I2C: PB6 -- SCL; PB7 -- SDA */
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C????????
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	I2C_DeInit(I2C1);//???I2C1
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//??????I2C???,???§Õ??
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

//	I2C_Cmd(I2C1, ENABLE);
//	I2C_Init(I2C1, &I2C_InitStructure);
//}

//void I2C_WriteByte(uint8_t addr,uint8_t data)
//{
//  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//	
//	I2C_GenerateSTART(I2C1, ENABLE);//????I2C1
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,????*/

//	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//??????? -- ???0x78
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//	I2C_SendData(I2C1, addr);//????????
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//	I2C_SendData(I2C1, data);//????????
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//	
//	I2C_GenerateSTOP(I2C1, ENABLE);//???I2C1????
//}

//void WriteCmd(unsigned char   I2C_Command)//§Õ????
//{
//	I2C_WriteByte(0x00, I2C_Command);
//}

//void WriteDat(unsigned char I2C_Data)//§Õ????
//{
//	I2C_WriteByte(0x40, I2C_Data);
//}

extern I2C_HandleTypeDef hi2c1;
void WriteCmd(unsigned char I2C_Command)//§Õ????     &hi2c1
{
//    uint8_t pData[1];
//    pData[0] = I2C_Command;
    uint8_t *pData;
    pData = &I2C_Command;
	HAL_I2C_Mem_Write(&hi2c1,OLED_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,pData,1,100);
}

void WriteDat(unsigned char I2C_Data)//§Õ???? 
{
//    uint8_t pData[1];
//    pData[0] = I2C_Data;
        uint8_t *pData;
    pData = &I2C_Data;
	HAL_I2C_Mem_Write(&hi2c1,OLED_ADDRESS,0x40,I2C_MEMADD_SIZE_8BIT,pData,1,100);
}


void OLED_Init(void)
{
	HAL_Delay(200); //?????????????
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	//WriteCmd(0xc8);	//COM?????õÕ???COM[N-1]??COM0????????Ñk???????????
	WriteCmd(0xc0);	//COM?????õÕ???COM0??COM[N-1]????????Ñk?????
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //??????? 0x00~0xff
	//WriteCmd(0xa1); //?§Ö??127???SEG0??????????????
	WriteCmd(0xa0);  //?§Ö??0???SEG0??????????
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //?????????????
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//??????
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//????
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ??OLED???????§Ý???
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //???????
	WriteCmd(0X14);  //????????
	WriteCmd(0XAF);  //OLED????
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ??OLED???? -- ????????,OLED???????10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //???????
	WriteCmd(0X10);  //??????
	WriteCmd(0XAE);  //OLED????
}





//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ?????????(x:0~127, y:0~7); ch[] -- ???????????; TextSize -- ?????§³(1:6*8 ; 2:8*16)
// Description    : ???codetab.h?§Ö?ASCII???,??6*8??8*16?????
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- ?????????(x:0~127, y:0~7); N:??????codetab.h?§Ö?????
// Description    : ???codetab.h?§Ö????,16*16????
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ?????????(x0:0~127, y0:0~7); x1,y1 -- ???????(??????)??????(x1:1~128,y1:1~8)
// Description    : ???BMP¦Ë?
//--------------------------------------------------------------

void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
 //  OLED??????????
//  "  ?¦Ë?????  ??
//  "????¦Ë??*****???
//  "????¦Ë??0.7  ???
//  "  ?????????????

void  OLED_Disp_Init(void)
   {
	unsigned char i;
      OLED_ShowStr(0,0,"OLED",2);  //"OLED"
		 for(i=0;i<6;i++)  
		{
			OLED_ShowCN(i*16+32,0,i+2);//"????????ïB
			
		}
for(i=0;i<6;i++)
		{
			OLED_ShowCN(i*16,2,i+8);//???????? ???????			
		}
	
		

	
}	
 
	//??OLED???¦Ë????????16¦Ë???????,5¦Ë???
//x,y -- ?????????(x:0~127, y:0~7); ch[] -- ???????????; TextSize -- ?????§³(1:6*8 ; 2:8*16)
 void OLED_ShowNumber_16(unsigned char x,unsigned char y,unsigned short dat)
{
	unsigned char p[6];

	p[0]= dat/10000%10+'0'; 
	p[1]= dat/1000%10+'0';
	p[2]= dat/100%10+'0'; 
	p[3]= dat/10%10+'0';
	p[4]= dat/1%10+'0';
	p[5]= '\0';
	if((dat/10000%10)==0)   //???????????0??
	{
	p[0]= dat/10000%10+' '; 
  }
 
	if(((dat/10000%10)==0) &&((dat/1000%10)==0))
	{
		
		 	p[1]=dat/1000%10+' ';
	}
	if(((dat/10000%10)==0) &&((dat/1000%10)==0)&&((dat/100%10)==0))		
	{	
		 	p[2]= dat/100%10+' ';		
	}
		
	if( ((dat/10000%10)==0) &&((dat/1000%10)==0)&&((dat/100%10)==0)&&((dat/10%10)==0) )		
	{	
		 	p[3]= dat/100%10+' ';		
	}
	OLED_ShowStr(x,y,p,2);	
	
}
void OLED_LAB_DISP_name(void){
	
		//char* intrr_cnt;
		//snprintf(intrr_cnt, 5,"%d",inter_cnt);
		//OLED_ShowCN(0, 0, 0); // show name
		//OLED_ShowCN(16, 0, 1); // show name
		//OLED_ShowCN(32, 0, 2); // show name
		//OLED_ShowStr(0, 1,"not pressed yet",1);
	  OLED_ShowStr(0,3,"20221689",2);  //"OLED"

}
void OLED_LAB_DISP(void){
		//int a  = 1;
		//char* intrr_cnt;
	OLED_ShowNumber_16(0,4, inter_cnt);
		//snprintf(intrr_cnt, 5,"%d",a);
		//OLED_ShowStr(0,3, intrr_cnt,1);  // interuppt cnt

}
void s2_pressed(void){
	OLED_ShowStr(0,0,"S2 pressed",2);  //"OLED"
	OLED_ShowCN(0, 3, 0); // show name
	OLED_ShowCN(16, 3, 1); // show name
	OLED_ShowCN(32, 3, 2); // show name
}
void s3_pressed(void){
	OLED_ShowStr(0,0,"S3 pressed",2);  //"OLED"
	OLED_ShowStr(0,3,"20221689",2);
}