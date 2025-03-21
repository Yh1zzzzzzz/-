#include "sw_lcd.h"
#include "stm32f1xx_hal.h"


//ST7920中文12864液晶，串行数据传输驱动方式
//#include "config.h"

#include "main.h"
//接线
//带汉字库12864液晶，PSB接低
//3线串行驱动，CS可直接接高，变成2线。
 //1. PS  接GND 
 //2. RS=CS=PB3;
 //3. RW=SDA = PB4;
 //4. E=SCK = PB5;
//  共4根线，占3个I/O口，若RS接地，则只占用PB4,PB5这两个I/O口
//  RS=CS;RW=SDA;E=SCK
 unsigned char number[]="0123456789";

//自定义不精确微秒延时，
//void My_delay_us(unsigned int us)
//{
//	unsigned short i = 0;
//	while(us--)
//	{
//		i = 7;
//		while(i--);
//	}
//}
//较精确微秒延时函数
void delayMicroseconds(uint32_t us)
{
    __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}


unsigned int Strlen(unsigned char *p)
{
	unsigned int i;
	for(i=0;*(p+i);i++);
	return i;
}

////宏定义CS 和SCK信号,已在Serial12864.h中定义
//#define CS_H_SW   HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);
//#define CS_L_SW   HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);
//#define SCK_H  HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, 1);
//#define SCK_L  HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, 0);
//GPIO模拟串行写一个字节
void WriteBytes(unsigned char Data)
{
	unsigned char i;
		for(i=0;i<8;i++)
	{
	// SDA_Pin 数据写入
		//HAL_GPIO_WritePin(LCD12864_SDA_GPIO_Port, LCD12864_SDA_Pin, (Data & 0x80)); 
	  if(Data&0x80)  HAL_GPIO_WritePin(LCD12864_SDA_GPIO_Port, LCD12864_SDA_Pin, GPIO_PIN_SET);
					  else   HAL_GPIO_WritePin(LCD12864_SDA_GPIO_Port, LCD12864_SDA_Pin, GPIO_PIN_RESET);

		SCK_L; 
	  SCK_L; 
    SCK_L; 
		SCK_L; //经测试，4条SCK_L指令时间刚好能正常显示
		
		// delayMicroseconds(1);//延时1微秒
		SCK_H ;//上升沿写入数据
		Data<<=1;
	}
}



//写一个字节的数据到液晶显示器(写3次）
void WriteData(unsigned char *p)
{
	unsigned char j;
	for(j=0;j<3;j++)
	{
		WriteBytes(p[j]);
	}
}


//重组待写的一个字节数据，并将此数据写入液晶显示器，
void SerialWriteData(unsigned char send)
{
	unsigned char temp[3];
	temp[0]=0xfa;                    //1111 1010向液晶写数据
	temp[1]=send & 0xf0;      //取高4位
	temp[2]=(send<<4) & 0xf0;       //取低4位
	CS_H_SW;  //片选置高
	
	WriteData(temp);
	
	CS_L_SW;  //片选置低
//delayMicroseconds(1);
	//delay_us(1);//串行不支持读操作，不可检测忙操作，这里用延时替代  
}

//串行写入一个"数据"到LCD12864
   void SerialWriteData2(unsigned char dat)
{
	CS_H_SW;  //片选置高	
	WriteBytes(0xfa);//1111 1010向液晶写“数据”
	WriteBytes(dat&0xf0);
	WriteBytes((dat<<4)&0xf0);	
  CS_L_SW;  //片选置低
}




//串行写入一个命令函数2
   void SerialWriteCmd2(unsigned char dat)
{
	
	CS_H_SW;  //片选置高
	
	WriteBytes(0xf8);
	WriteBytes(dat&0xf0);
	WriteBytes((dat<<4)&0xf0);
	
CS_L_SW;  //片选置低
 // delayMicroseconds(1);
	// delay_us(1);//串行不支持读操作，不可检测忙操作，这里用延时替代  
}



////重组待写的一个字节命令的数据，并将此命令数据写入液晶显示器，
void SerialWriteCmd(unsigned char send)
{
	unsigned char temp[3];
	temp[0]=0xf8 ;                    //1111 1000向液晶写命令
	temp[1]=send & 0xf0;      //取高4位
	temp[2]=(send<<4) & 0xf0;       //取低4位
	CS_H_SW;  //片选置高
	
	WriteData(temp);	
	
	CS_L_SW;  //片选置低
	//delayMicroseconds(1);
	//delay_us(1);//串行不支持读操作，不可检测忙操作，这里用延时替代  
}


		//进入绘图模式
	void	drawon(void)
		{
    	SerialWriteCmd(0x36);//打开扩展指令集，绘图显示开
			delayMicroseconds(100);
			//delay_us(100);
		}			

		//退出绘图模式	
	void	drawoff(void)
		{
      SerialWriteCmd(0x34);//打开扩展指令集，绘图显示关
      	delayMicroseconds(100);
			//delay_us(100);
//			 SerialWriteCmd(0x30);//回到基本指令集
//			delay_us(100);
		}			
			
//设置显示位置,上半屏
void SerialSetPosition(unsigned char page,unsigned char column)
{
	//column=column;
	//SerialWriteCmd2(0x40+page);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x80+page);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x80+column);  //设置列地址  绘图模式下列地址为0X80+ 0--16
	 
}
void SerialDraw_Clr(void)

{
	unsigned char i,j;
	SerialWriteCmd(0x34);//打开扩展指令集，即使是36H，也要写2次
		delayMicroseconds(100);
	//delay_us(100);
	SerialWriteCmd(0x36);//打开扩展指令集，开绘图
		delayMicroseconds(100);
	//delay_us(100);
	for(i=0;i<32;i++)
	   {
			 SerialWriteCmd(0x80+i);//设置列位置
			  SerialWriteCmd(0x80);// 设置行位置
			 
			    for(j=0;j<32;j++)
			       {
							SerialWriteData2(0);
							
						 }
			 
		 }
	drawoff();
		 SerialWriteCmd(0x30);//回到基本指令集
			delayMicroseconds(100);
		 // delay_us(100);
		 //delay_ms(3000);
}



		/*************************************************************************
 Function    : 显示一张图片  32*32
 Description : 四行高度 每行宽32
**************************************************************************/
void SerialShow_picture3232(unsigned char line,unsigned char column,unsigned char *address)
{
	unsigned char i,j;
	
	 //SerialDraw_Clr();
	  drawoff();
		drawon();
	for(i=0;i<32;i++)  //32  先清除相应屏幕
	  {
			//SerialSetPosition(line+i, column);
		
  SerialWriteCmd2(0x80+line+i);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x80+column);  //设置列地址  绘图模式下列地址为0X80+ 0--15，0--7：上半屏，8--15：下半屏
			
	    for(j=0;j<4;j++)                  // 宽度32位 //4
	       {
	        //num=*address;
					 SerialWriteData2(0);
	       }
			
		}
	
		for(i=0;i<32;i++)  //32  
	  {
			//SerialSetPosition(line+i, column);
		
  SerialWriteCmd2(0x80+line+i);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x80+column);  //设置列地址  绘图模式下列地址为0X80+ 0--15，0--7：上半屏，8--15：下半屏
			
	    for(j=0;j<4;j++)                  // 宽度32位 //4
	       {
	       // num=*address;
					 SerialWriteData2(*(address++));
	       }
			
		}
		
		 SerialWriteCmd(0x30);//回到基本指令集
			delayMicroseconds(100);
		//delay_us(100);
	//	delay_ms(3);
		
		
	}	
	/*************************************************************************
 Function    : 显示一张图片  128*64
 Description : 32行高度 每行宽256
**************************************************************************/

		void SerialShow_picture12864(unsigned char line,unsigned char column,unsigned char *address)
{


	unsigned char i,j;
	
	 //SerialDraw_Clr();
	  drawoff();
		drawon();
	for(i=0;i<32;i++)  //32  先清除相应屏幕
	  {
			//SerialSetPosition(line+i, column);
		
  SerialWriteCmd2(0x80+line+i);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x80+column);  //设置列地址  绘图模式下列地址为0X80+ 0--15，0--7：上半屏，8--15：下半屏
			
	    for(j=0;j<32;j++)                  // 宽度32位 //4
	       {
	        //num=*address;
					 SerialWriteData2(0);
	       }
			
		}
		
	
for(i=0;i<32;i++)  //写上半屏
	  {
			//SerialSetPosition(line+i, column);
		
  SerialWriteCmd2(0x80+line+i);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x80+column);  //设置列地址  绘图模式下列地址为0X80+ 0--16
			
	    for(j=0;j<16;j++)                  // 宽度32位 //4
	       {
	     //   num=*address;
					 SerialWriteData2(*(address++));
	       }
			
		}
    
		for(i=0;i<32;i++)  //写下半屏
	  {
			//SerialSetPosition(line+i, column);
		
  SerialWriteCmd2(0x80+line+i);  //设置行地址  绘图模式下行地址为0X40+ 0--32
	SerialWriteCmd2(0x88+column);  //设置列地址  绘图模式下列地址为0X80+ 0--16
			
	    for(j=0;j<32;j++)                  // 宽度32位 //4
	       {
	     //   num=*address;
					 SerialWriteData2(*(address++));
	       }
			
		}
		
		
		//drawon();
		//drawoff();
		

		SerialWriteCmd(0x30);//回到基本指令集
	  	delayMicroseconds(100);
		//	delay_us(100);
		
		
}

/************************************/
 //打点绘图  一次打水平一行     可以避免断点现象
 //x表示数组的首地址，y表示纵坐标的值，也即是表示第多少行
//即对一个数组中的数进行这样的处理：
//检测数组，并默认数组为一行数的记录即128字节，只要数组中有数等于y，就把第y行的数全部打出
/************************************/
void Serial_draw_word(unsigned char *x, unsigned char y1) 
 {
  unsigned char i,j,k,m,n,count=0;
   unsigned char hdat, ldat;         
   unsigned char y_byte, y_bit;    //在纵坐标的哪一个字节，哪一个位
    unsigned char   a[16];
   SerialWriteCmd(0x34);         //打开扩展指令集
	  SerialWriteCmd(0x36);    
    y_byte=y1/32;                    //y是没在哪个字节这个说法
    y_bit=y1%32;                    //y_bit确定它是在第几行
   for(j=0;j<8;j++)
 {
  hdat=0, ldat=0;     //此处清零是很必要的
  n=j*16;
  for(k=n;k<n+16;k++)
   {
   if(x[k]==y1)
      {
    a[count]=k;
    count++;
    }
    } 
 for(m=0;m<count;m++)
   {
    i=a[m]-n;
       if(i<8)                              //如果x_bit位数小于8
          hdat=hdat|(0x01<<(7-i));    //写高字节。因为坐标是从左向右的
       else
        ldat=ldat|(0x01<<(15-i));
      }
   
   SerialWriteCmd(0x80+y_bit);       //垂直地址(上)   貌似与说明书正好相反
   SerialWriteCmd(0x80+j+8*y_byte);  //水平坐标（下）   貌似与说明书正好相反   
   SerialWriteData2(hdat);
    SerialWriteData2(ldat);
			count=0;//计数器淸0，
  }
   //SerialWriteCmd(0x36);                     //打开绘图显示
	//delay_us(100);
   SerialWriteCmd(0x30);               //回到基本指令集
		delayMicroseconds(100);
	//delay_us(100);
 }
/*----------------------------------------------------------------------------------------------------------------------*/
/******************************************************/
//画正弦波的波形
/******************************************************/
void Serial_print_sinx(void)
{
 unsigned char i;
 unsigned char  y_sin[128];   //定义屏幕上要打的正弦波的纵坐标  //XDATA
    unsigned char  v[128]={32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,};
 float y;
 for(i=0;i<128;i++)
 {
  //y=31*sin(0.09*i)+0.5;   //此处系数为31比较好
 
	 y_sin[i]=32-y;
	
 }
  for(i=0;i<64;i++)     
  {
    Serial_draw_word(y_sin, i);   //绘图  一行一行绘
		
	}
    Serial_draw_word(v, 32);//32
    
   }
	  


 
		void InitLcd(void)
{
	HAL_Delay(50);//大于40MS的延时
			            
		//psb=0;//设置为8BIT并口工作模式，0：串口模式,硬件直接接地，串行模式
		 
	//delay_ms(5);//delay_50us(100);              //延时
		CS_H_SW;  //片选置高
  
	HAL_Delay(5);//delay_50us(100);              //延时
        
	  SerialWriteCmd(0x30);//选择基本指令集 
	  	HAL_Delay(1);
		// delay_us(50);   //          ////大于37uS的延时
	
		SerialWriteCmd(0x0C);//开显示，无游标，不反白
	  HAL_Delay(1);
	// delay_us(110);//           //大于100uS的延时
	
	  
		SerialWriteCmd(0x10);//Cursor Display Control光标设置
		HAL_Delay(1);
	//delay_us(110);//     //大于100uS的延时程序
		
	
		SerialWriteCmd(0x01);//清除显示屏幕，把DDRAM位址计数器调整为00H
	HAL_Delay(10);
	//	delay_ms(10);//   //大于10mS的延时,清屏时间较长，
	
	
//	SerialWriteCmd(0x06);//设定光标右移，整体显示不移动
	  SerialWriteCmd(0x14);//设定光标右移，AC加1，整体显示不移动
    HAL_Delay(1);
//	delay_us(110);//    //大于100uS的延时
	CS_L_SW;  //片选置低
	//CS=0;

}



void DisplayAt(unsigned char hang,unsigned char lie,unsigned char *p)
{
	unsigned char i;
	switch(hang)
	{
		case 1:SerialWriteCmd2(0x80+lie);break;
		case 2:SerialWriteCmd2(0x90+lie);break;
		case 3:SerialWriteCmd2(0x88+lie);break;
		case 4:SerialWriteCmd2(0x98+lie);break;
	}
	for(i=0;i<Strlen(p);i++)
	{
		SerialWriteData2(p[i]);
	}
}


void DisplayAt2(unsigned char hang,unsigned char lie,unsigned char *p)
{
	//unsigned char i;
	switch(hang)
	{
		case 1:SerialWriteCmd2(0x80+lie);break;
		case 2:SerialWriteCmd2(0x90+lie);break;
		case 3:SerialWriteCmd2(0x88+lie);break;
		case 4:SerialWriteCmd2(0x98+lie);break;
	}
	while(*p!='\0')
	{
		SerialWriteData2(*p);
		p++;
	}
//	for(i=0;i<Strlen(p);i++)
//	{
//		SerialWriteData2(p[i]);
//	}
}

void Dis( unsigned char *p)
{
	unsigned char i;
	for(i=0;i<Strlen(p);i++)
	{
		SerialWriteData2(p[i]);
	}
}

void ChangeCR(unsigned char hang,unsigned char lie)
{
	switch(hang)
	{
		case 1:SerialWriteCmd(0x80+lie);break;
		case 2:SerialWriteCmd(0x90+lie);break;
		case 3:SerialWriteCmd(0x88+lie);break;
		case 4:SerialWriteCmd(0x98+lie);break;
	}
}


//在指定位置显示一个16位无符号数

 void ShowNumber_16(unsigned char hang,unsigned char lie,unsigned short dat)
{
	unsigned char p[5];
	
	switch(hang)
	{
		case 1:SerialWriteCmd(0x80+lie);break;
		case 2:SerialWriteCmd(0x90+lie);break;
		case 3:SerialWriteCmd(0x88+lie);break;
		case 4:SerialWriteCmd(0x98+lie);break;
	}
	
	p[0]= dat/10000%10+'0'; 
	p[1]= dat/1000%10+'0';
	p[2]= dat/100%10+'0'; 
	p[3]= dat/10%10+'0';
	p[4]= dat/1%10+'0';
	
	if((dat/10000%10)==0)   //消隐最前面的“0”
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
	
	SerialWriteData(p[0]);
	SerialWriteData(p[1]);
	SerialWriteData(p[2]);
	SerialWriteData(p[3]);
	SerialWriteData(p[4]);
	

}



void Write_CQU(void){
//中文位置打印学号
    DisplayAt(0,1,"重庆大学");
}
void Write_automation(void){
//中文位置
    DisplayAt(1,0,"自动化学院");
}

void Write_xuehao(void){
//中文位置打印
    DisplayAt(2,0,"二零二二一六八九");
}
void Write_xuehao_eng(void){
//英语位置打印学号
    DisplayAt(3,2,"2021689");
}
void LCD_Display_SW(void){
	DisplayAt(1,0,"current:");
	ShowNumber_16(2,8,curr_key);
	DisplayAt(4,0,(char *)history_key);

}
