#include "AD1674.h"
#include "main.h"

/**
 * @brief 初始化AD1674 ADC
 */
void AD1674_Init(void)
{
    // 设置CS引脚为高电平（非选中状态）
    HAL_GPIO_WritePin(AD1674_CS_GPIO_Port, AD1674_CS_Pin, GPIO_PIN_SET);
    
    // 设置CE引脚为高电平（非使能状态）
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
    
    // 设置RC引脚为高电平（初始状态）
    HAL_GPIO_WritePin(RC_GPIO_Port, RC_Pin, GPIO_PIN_SET);
    
    // 设置A0引脚为高电平（初始状态，设置为12位模式）
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_SET);

    //  设置I2_8引脚为高电平（初始状态）
    HAL_GPIO_WritePin(I2_8_GPIO_Port, I2_8_Pin, GPIO_PIN_SET);
    
}

/**
 * @brief 启动ADC转换
 */
void AD1674_StartConversion(void)
{
    // 选中芯片
    HAL_GPIO_WritePin(AD1674_CS_GPIO_Port, AD1674_CS_Pin, GPIO_PIN_RESET);
    
    // 芯片使能
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
    
    // 触发转换 - 发送下降沿脉冲到RC引脚
    HAL_GPIO_WritePin(RC_GPIO_Port, RC_Pin, GPIO_PIN_RESET);
    HAL_Delay(1); // 短暂延时
    HAL_GPIO_WritePin(RC_GPIO_Port, RC_Pin, GPIO_PIN_SET);
}

/**
 * @brief 检查转换是否完成
 * @return 1 - 转换完成，0 - 转换中
 */
uint8_t AD1674_IsConversionDone(void)
{
    // 当STS引脚为低电平时，表示转换完成
    return (HAL_GPIO_ReadPin(STS_GPIO_Port, STS_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}

/**
 * @brief 读取转换结果
 * @return 12位ADC值
 */
uint16_t AD1674_ReadValue(void)
{
    uint16_t adcValue = 0;
    
    // 读取高8位数据
    adcValue |= HAL_GPIO_ReadPin(DB7_GPIO_Port, DB7_Pin) << 11;
    adcValue |= HAL_GPIO_ReadPin(DB6_GPIO_Port, DB6_Pin) << 10;
    adcValue |= HAL_GPIO_ReadPin(DB5_GPIO_Port, DB5_Pin) << 9;
    adcValue |= HAL_GPIO_ReadPin(DB4_GPIO_Port, DB4_Pin) << 8;
    adcValue |= HAL_GPIO_ReadPin(DB3_GPIO_Port, DB3_Pin) << 7;
    adcValue |= HAL_GPIO_ReadPin(DB2_GPIO_Port, DB2_Pin) << 6;
    adcValue |= HAL_GPIO_ReadPin(DB1_GPIO_Port, DB1_Pin) << 5;
    adcValue |= HAL_GPIO_ReadPin(DB0_GPIO_Port, DB0_Pin) << 4;
    
    // 切换地址以读取低4位
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_RESET);
    
    // 短暂延时
    HAL_Delay(1);
    
    // 读取低4位数据
    adcValue |= HAL_GPIO_ReadPin(DB7_GPIO_Port, DB7_Pin) << 3;
    adcValue |= HAL_GPIO_ReadPin(DB6_GPIO_Port, DB6_Pin) << 2;
    adcValue |= HAL_GPIO_ReadPin(DB5_GPIO_Port, DB5_Pin) << 1;
    adcValue |= HAL_GPIO_ReadPin(DB4_GPIO_Port, DB4_Pin);
    
    // 恢复A0为高电平
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_SET);
    
    // 取消芯片使能
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
    
    // 取消芯片选中
    HAL_GPIO_WritePin(AD1674_CS_GPIO_Port, AD1674_CS_Pin, GPIO_PIN_SET);
    
    return adcValue;
}

/**
 * @brief 完整ADC读取过程 - 启动转换并等待结果
 * @return 12位ADC值
 */
uint16_t AD1674_ReadADC(void)
{
    // 启动转换
    AD1674_StartConversion();
    
    // 等待转换完成
    while (!AD1674_IsConversionDone());
    
    // 读取并返回结果
    return AD1674_ReadValue();
}