#include "tm1650.h"
#include "stm32f1xx_hal.h"
// 全局变量定义

// 数码管段码表 (0-9, A-F)
const uint8_t TM1650_SEGMENT_MAP[16] = {
    0x3F, 0x06, 0x5B, 0x4F,  // 0, 1, 2, 3
    0x66, 0x6D, 0x7D, 0x07,  // 4, 5, 6, 7
    0x7F, 0x6F, 0x77, 0x7C,  // 8, 9, A, B
    0x39, 0x5E, 0x79, 0x71   // C, D, E, F
};

// 延时函数
static void TM1650_Delay(void) {
    for(volatile int i = 0; i < 50; i++);
}

// 设置SDA为输入
static void TM1650_SDA_Input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SW_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SW_SDA_GPIO_Port, &GPIO_InitStruct);
}

// 设置SDA为输出
static void TM1650_SDA_Output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SW_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SW_SDA_GPIO_Port, &GPIO_InitStruct);
}

// 读取SDA状态
static uint8_t TM1650_SDA_Read(void) {
    return HAL_GPIO_ReadPin(SW_SDA_GPIO_Port, SW_SDA_Pin);
}

// 开始信号
static void TM1650_Start(void) {
    TM1650_SDA_Output();
    TM1650_SDA_1;
    TM1650_SCL_1;
    TM1650_Delay();
    TM1650_SDA_0;
    TM1650_Delay();
    TM1650_SCL_0;
    TM1650_Delay();
}

// 停止信号
static void TM1650_Stop(void) {
    TM1650_SDA_Output();
    TM1650_SCL_0;
    TM1650_SDA_0;
    TM1650_Delay();
    TM1650_SCL_1;
    TM1650_Delay();
    TM1650_SDA_1;
    TM1650_Delay();
}

// 应答信号
static void TM1650_Ack(void) {
    TM1650_SCL_0;
    TM1650_SDA_Output();
    TM1650_SDA_0;
    TM1650_Delay();
    TM1650_SCL_1;
    TM1650_Delay();
    TM1650_SCL_0;
    TM1650_Delay();
}

// 写字节
static void TM1650_WriteByte(uint8_t data) {
    TM1650_SDA_Output();
    
    for (uint8_t i = 0; i < 8; i++) {
        TM1650_SCL_0;
        TM1650_Delay();
        
        if (data & 0x80) {
            TM1650_SDA_1;
        } else {
            TM1650_SDA_0;
        }
        
        data <<= 1;
        TM1650_Delay();
        TM1650_SCL_1;
        TM1650_Delay();
    }
    
    // 读取应答信号
    TM1650_SCL_0;
    TM1650_SDA_Input();
    TM1650_Delay();
    TM1650_SCL_1;
    TM1650_Delay();
    TM1650_SCL_0;
}

// 读字节
static uint8_t TM1650_ReadByte(void) {
    uint8_t data = 0;
    
    TM1650_SDA_Input();
    
    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        TM1650_SCL_0;
        TM1650_Delay();
        TM1650_SCL_1;
        TM1650_Delay();
        
        if (TM1650_SDA_Read()) {
            data |= 0x01;
        }
    }
    
    // 发送应答
    TM1650_Ack();
    
    return data;
}

// 写命令+数据
static void TM1650_WriteCmd(uint8_t cmd, uint8_t data) {
    TM1650_Start();
    TM1650_WriteByte(cmd);
    TM1650_WriteByte(data);
    TM1650_Stop();
}

// 初始化TM1650
void TM1650_Init(void) {
    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 配置CLK为输出
    GPIO_InitStruct.Pin = SW_SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SW_SCK_GPIO_Port, &GPIO_InitStruct);
    
    // 配置CS为输出
    GPIO_InitStruct.Pin = SW_CS_Pin;
    HAL_GPIO_Init(SW_CS_GPIO_Port, &GPIO_InitStruct);
    
    // 配置SDA为输出
    TM1650_SDA_Output();
    
    // 设置初始电平
    TM1650_SCL_1;
    TM1650_SDA_1;
    HAL_GPIO_WritePin(SW_CS_GPIO_Port, SW_CS_Pin, GPIO_PIN_SET);
    
    // 配置TM1650：显示开，键盘扫描开，中等亮度
    TM1650_WriteCmd(TM1650_DISPLAY_MODE_ADDR, 
                   TM1650_DISPLAY_ON | TM1650_KEYBOARD_ON | TM1650_NORMAL_MODE | (4 << 4));
    
    // 清除所有显示
    TM1650_WriteCmd(TM1650_DISPLAY_REG1_ADDR, 0x00);
    TM1650_WriteCmd(TM1650_DISPLAY_REG2_ADDR, 0x00);
    TM1650_WriteCmd(TM1650_DISPLAY_REG3_ADDR, 0x00);
    TM1650_WriteCmd(TM1650_DISPLAY_REG4_ADDR, 0x00);
}

// 设置亮度
void TM1650_SetBrightness(uint8_t brightness) {
    if (brightness > 7) brightness = 7;
    
    TM1650_WriteCmd(TM1650_DISPLAY_MODE_ADDR, 
                   TM1650_DISPLAY_ON | TM1650_KEYBOARD_ON | TM1650_NORMAL_MODE | (brightness << 4));
}

// 显示单个数字
void TM1650_DisplayDigit(uint8_t position, uint8_t digit, uint8_t dot) {
    uint8_t segData;
    
    if (digit > 15) digit = 0;  // 限制数字范围
    if (position > 4 || position < 1) return;  // 位置范围检查
    
    segData = TM1650_SEGMENT_MAP[digit];
    if (dot) segData |= 0x80;  // 添加小数点
    
    switch (position) {
        case 1:
            TM1650_WriteCmd(TM1650_DISPLAY_REG1_ADDR, segData);
            break;
        case 2:
            TM1650_WriteCmd(TM1650_DISPLAY_REG2_ADDR, segData);
            break;
        case 3:
            TM1650_WriteCmd(TM1650_DISPLAY_REG3_ADDR, segData);
            break;
        case 4:
            TM1650_WriteCmd(TM1650_DISPLAY_REG4_ADDR, segData);
            break;
    }
}

// 显示数字
void TM1650_DisplayNumber(uint16_t number) {
    if (number > 9999) number = 9999;
    
    TM1650_DisplayDigit(1, number / 1000, 0);
    TM1650_DisplayDigit(2, (number / 100) % 10, 0);
    TM1650_DisplayDigit(3, (number / 10) % 10, 0);
    TM1650_DisplayDigit(4, number % 10, 0);
}

// 读取按键
uint8_t TM1650_ReadKey(void) {
    uint8_t keyValue = 0;
    
    TM1650_Start();
    TM1650_WriteByte(TM1650_KEY_SCAN_ADDR);
    keyValue = TM1650_ReadByte();
    TM1650_Stop();
    
    // 将TM1650按键编码转换为1-16的键值
    if (keyValue == 0) return 0;  // 无按键
    
    // 解析TM1650的键值
    // K1-K8编码对应的按键位置
    uint8_t row = 0, col = 0;
    
    // 找出哪一行被按下 (K5-K8)
    if (keyValue & 0x01) row = 1;      // K8
    else if (keyValue & 0x02) row = 2;  // K7
    else if (keyValue & 0x04) row = 3;  // K6
    else if (keyValue & 0x08) row = 4;  // K5
    
    // 找出哪一列被按下 (K1-K4)
    if (keyValue & 0x10) col = 1;      // K4
    else if (keyValue & 0x20) col = 2;  // K3
    else if (keyValue & 0x40) col = 3;  // K2
    else if (keyValue & 0x80) col = 4;  // K1
    
    // 如果行列都有效，计算键值(1-16)
    if (row > 0 && col > 0) {
        return (row - 1) * 4 + col;
    }
    
    return 0;  // 无有效按键
}

// 将键值转换为字符并记录到历史缓冲区
void TM1650_DisplayKeyValue(uint8_t keyValue) {
    if (keyValue == 0) return;
    
    // 更新当前按键
    curr_key = keyValue;
    
    // 将当前按键值转换为字符并存入历史记录
    char keyChar;
    if (keyValue <= 9) {
        keyChar = '0' + keyValue;  // 1-9 -> '1'-'9'
    } else {
        keyChar = 'A' + (keyValue - 10);  // 10-16 -> 'A'-'G'
    }
    
    // 将按键字符添加到历史记录
    history_key[index_key] = keyChar;
    index_key = (index_key + 1) % history_key_size;
    
    // 在第3位数码管上显示按键值
    TM1650_DisplayDigit(3, keyValue <= 9 ? keyValue : (keyValue - 10 + 10), 0);
}