
#define R1_Pin GPIO_PIN_0
#define R1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_1
#define R2_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_2
#define R3_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_10
#define R4_GPIO_Port GPIOB
#define C1_Pin GPIO_PIN_15
#define C1_GPIO_Port GPIOA
#define C2_Pin GPIO_PIN_10
#define C2_GPIO_Port GPIOC
#define C3_Pin GPIO_PIN_11
#define C3_GPIO_Port GPIOC
#define C4_Pin GPIO_PIN_12
#define C4_GPIO_Port GPIOC
extern unsigned char  curr_key;
extern int lcd_flag;
extern int PWM_pulse;
#define history_key_size 16
extern unsigned char history_key[history_key_size];
extern int index_key;
int scan_KeyBoard();
void Change_motor_direction(int direction);
void Moteor_Speed_change(int falg);
extern TIM_HandleTypeDef htim3;
extern int Moter_direction;