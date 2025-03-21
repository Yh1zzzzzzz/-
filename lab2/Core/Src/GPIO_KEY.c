#include "key.h"
#include "stm32f1xx_hal.h"
#define S3_Pin GPIO_PIN_7
#define S3_GPIO_Port GPIOC
#define S1_Pin GPIO_PIN_8
#define S1_GPIO_Port GPIOC
#define S4_Pin GPIO_PIN_9
#define S4_GPIO_Port GPIOC
#define S5_Pin GPIO_PIN_9
#define S5_GPIO_Port GPIOB
/**
 * @brief 按键扫描函数 - 采用延时去抖动方法
 * 
 * 该函数检测四个按键(S1, S3, S4, S5)的状态，使用延时去抖动技术
 * 确保按键输入的可靠性。当检测到按键按下时(电平不为0)，进行延时
 * 20ms后再次检测，如果状态保持不变，则确认为有效按键事件。
 *
 * @note 该函数使用外部定义的keysta数组记录按键状态
 * @note 按键处于按下状态时返回对应的按键编号
 * @note 检测顺序为S1->S3->S4->S5，一次只能检测到一个按键
 * @note 函数使用阻塞延时，调用时需考虑其对实时性要求高的应用的影响
 *
 * @return int 按下的按键编号(1-4)，无按键按下时返回0
 *      - 0: 无按键按下
 *      - 1: S1按键按下
 *      - 2: S3按键按下
 *      - 3: S4按键按下
 *      - 4: S5按键按下
 */
int keysta[4] = {0, 0, 0, 0};
int key_scan(void){
    // 读取当前按键状态
    int key1 = HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin);
    int key2 = HAL_GPIO_ReadPin(S3_GPIO_Port, S3_Pin);
    int key3 = HAL_GPIO_ReadPin(S4_GPIO_Port, S4_Pin);
    int key4 = HAL_GPIO_ReadPin(S5_GPIO_Port, S5_Pin);
    
    // 检查按键1状态变化
    if (key1 != 0) {
        HAL_Delay(20); // 延时20ms去抖动
        // 再次读取确认状态稳定
        if (key1 == HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin)) {
            keysta[0] = key1; // 更新按键状态
            return 1;
        }
    }
    
    // 检查按键2状态变化
    if (key2 != 0) {
        HAL_Delay(20); // 延时20ms去抖动
        // 再次读取确认状态稳定
        if (key2 == HAL_GPIO_ReadPin(S3_GPIO_Port, S3_Pin)) {
            keysta[1] = key2; // 更新按键状态
            return 2;   
        }
    }
    
    // 检查按键3状态变化
    if (key3 != 0) {
        HAL_Delay(20); // 延时20ms去抖动
        // 再次读取确认状态稳定
        if (key3 == HAL_GPIO_ReadPin(S4_GPIO_Port, S4_Pin)) {
            keysta[2] = key3; // 更新按键状态
            return 3;
        }
    }
    
    // 检查按键4状态变化
    if (key4 != 0) {
        HAL_Delay(20); // 延时20ms去抖动
        // 再次读取确认状态稳定
        if (key4 == HAL_GPIO_ReadPin(S5_GPIO_Port, S5_Pin)) {
            keysta[3] = key4; // 更新按键状态
            return 4;
        }
    }
    return 0;
}