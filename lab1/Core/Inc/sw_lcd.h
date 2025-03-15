
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
void Write_Byte(unsigned char dat);
void Write_Cmd(unsigned char dat);
void Write_Data(unsigned char dat);
void LCD_Init(void);
void LCD_Display_Words(unsigned char x,unsigned char y,unsigned char*str);
void Write_CQU(void);
void Write_automation(void);
void Write_xuehao(void);
void LCD_Display_SW(void);