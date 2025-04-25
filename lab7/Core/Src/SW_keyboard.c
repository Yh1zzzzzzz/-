#include "sw_keyboard.h"
#include "stm32f1xx_hal.h"
#include "spi_lcd.h"
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
#define IN1_Pin GPIO_PIN_0
#define IN1_GPIO_Port GPIOC
#define IN2_Pin GPIO_PIN_1
#define IN2_GPIO_Port GPIOC

/**
 * @brief 矩阵键盘扫描函数
 * 
 * 通过双向扫描方法检测4x4矩阵键盘按键按下情况：
 * 1. 首先将列设为输出低电平，行设为输入上拉，检测哪一行为低电平
 * 2. 然后将行设为输出低电平，列设为输入上拉，检测哪一列为低电平
 * 3. 根据行列组合确定唯一按键位置
 * 4. 将按键值转换为字符并存入历史记录数组
 *
 * @note 该函数使用GPIO重新配置方式，在主循环中频繁调用可能会消耗较多CPU资源
 * @note 使用了HAL_Delay(10)确保信号稳定，这是阻塞式延时
 * @note 键值1-9映射为字符'1'-'9'，键值10-16映射为字符'A'-'G'
 * @note 使用环形缓冲区存储按键历史记录
 * 
 * @warning 该函数依赖全局变量curr_key、history_key和index_key
 * 
 * 键值对应关系：
 *   1  2  3  4
 *   5  6  7  8
 *   9  10 11 12
 *   13 14 15 16
 */
int scan_KeyBoard() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t row = 0, col = 0;
    
    // 第一步：检测行
    // 配置列为输出模式，输出低电平
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    GPIO_InitStruct.Pin = C1_Pin;
    HAL_GPIO_Init(C1_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = C2_Pin | C3_Pin | C4_Pin;
    HAL_GPIO_Init(C2_GPIO_Port, &GPIO_InitStruct);
    
    // 输出低电平
    HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, GPIO_PIN_RESET);
    
    // 配置行为输入模式
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    
    GPIO_InitStruct.Pin = R1_Pin | R2_Pin | R3_Pin | R4_Pin;
    HAL_GPIO_Init(R1_GPIO_Port, &GPIO_InitStruct);

    
    // 读取行线状态
    HAL_Delay(10); // 短暂延时等待信号稳定
    
    if (HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin) == GPIO_PIN_RESET) row = 1;
    else if (HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin) == GPIO_PIN_RESET) row = 2;
    else if (HAL_GPIO_ReadPin(R3_GPIO_Port, R3_Pin) == GPIO_PIN_RESET) row = 3;
    else if (HAL_GPIO_ReadPin(R4_GPIO_Port, R4_Pin) == GPIO_PIN_RESET) row = 4;
    
    if (row == 0) return 0 ; // 没有按键按下
    
    // 第二步：检测列
    // 配置行为输出模式，输出低电平
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    
    GPIO_InitStruct.Pin = R1_Pin;
    HAL_GPIO_Init(R1_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = R2_Pin;
    HAL_GPIO_Init(R2_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = R3_Pin;
    HAL_GPIO_Init(R3_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = R4_Pin;
    HAL_GPIO_Init(R4_GPIO_Port, &GPIO_InitStruct);
    
    // 输出低电平
    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET);
    
    // 配置列为输入模式
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    
    GPIO_InitStruct.Pin = C1_Pin;
    HAL_GPIO_Init(C1_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = C2_Pin;
    HAL_GPIO_Init(C2_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = C3_Pin;
    HAL_GPIO_Init(C3_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = C4_Pin;
    HAL_GPIO_Init(C4_GPIO_Port, &GPIO_InitStruct);
    
    // 读取列线状态
    HAL_Delay(10); // 短暂延时等待信号稳定
    
    if (HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin) == GPIO_PIN_RESET) col = 1;
    else if (HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin) == GPIO_PIN_RESET) col = 2;
    else if (HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin) == GPIO_PIN_RESET) col = 3;
    else if (HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin) == GPIO_PIN_RESET) col = 4;
    
    // 根据行列值确定按键
    if (col == 0) return 0; // 没有按键按下
    lcd_flag = 1;
		
    // 计算键值：(row-1)*4 + col
    int ret = (col-1)*4 + row;
    if (ret <= 10){
        curr_key = ret - 1  + '0';
        history_key[index_key] = curr_key;
        index_key = (index_key + 1) % history_key_size;
				//LCD_Display_Words(1,0,(unsigned char *)curr_key);
    }else{
        curr_key = ret - 11 + 'A';
        history_key[index_key] = curr_key;
        index_key = (index_key + 1) % history_key_size;
				//LCD_Display_Words(1,0,(unsigned char *)curr_key);

    }
    return 1;
}
void Change_motor_direction(int direction)
{
    if(direction == 0)
    {
      //正转
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
    }
    if(direction == 1)
    {
      //反转
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);
    }
    if(direction == 2)
    {
      //停止
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
    }
}
void Moteor_Speed_change(int falg)
{
    if(falg == 1)
    {
        //加速
        PWM_pulse += 100;
        if(PWM_pulse > 1000) PWM_pulse = 1000;
    }
    if(falg == 2)
    {
        //减速
        PWM_pulse -= 100;
        if(PWM_pulse <= 200) PWM_pulse = 200;
    }
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, PWM_pulse);
}