#include "dac0832.h"
#include "math.h"

// 正弦波查找表
#define SINE_POINTS 256
uint8_t sine_table[SINE_POINTS];
uint16_t sine_index = 0;

/**
 * @brief  初始化DAC0832
 */
void DAC0832_Init(void)
{
  // 确保GPIO端口已经在MX_GPIO_Init()中初始化
  
  // 初始化CS和WR为高电平
  HAL_GPIO_WritePin(DAC_CS_PORT, DAC_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(DAC_WR_PORT, DAC_WR_PIN, GPIO_PIN_SET);
}

/**
 * @brief  向DAC0832写入数据
 * @param  data: 要写入的8位数据
 */
void DAC0832_Write(uint8_t data)
{
  // 拉低CS信号，选中芯片
  HAL_GPIO_WritePin(DAC_CS_PORT, DAC_CS_PIN, GPIO_PIN_RESET);
  
  // 拉低WR信号，开始写入
  HAL_GPIO_WritePin(DAC_WR_PORT, DAC_WR_PIN, GPIO_PIN_RESET);
  
  // 逐位设置数据线
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D0_PIN, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D1_PIN, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D2_PIN, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D3_PIN, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D4_PIN, (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D5_PIN, (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D6_PIN, (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DAC_DATA_PORT, DAC_D7_PIN, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  
  // 等待一小段时间，确保数据稳定
  for(uint8_t i = 0; i < 10; i++);
  
  // 拉高WR信号，锁存数据
  HAL_GPIO_WritePin(DAC_WR_PORT, DAC_WR_PIN, GPIO_PIN_SET);
  
  // 拉高CS信号，结束写入
  HAL_GPIO_WritePin(DAC_CS_PORT, DAC_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief  初始化正弦波查找表
 */
void DAC0832_SineWave_Init(void)
{
  float pi = 3.14159265359;
  
  for (int i = 0; i < SINE_POINTS; i++)
  {
    // 生成0-255范围的正弦波
    float angle = (2.0 * pi * i) / SINE_POINTS;
    // 缩放到0-255范围：(sin(x) + 1) * 127.5
    sine_table[i] = (uint8_t)((sinf(angle) + 1.0) * 127.5);
  }
  
  sine_index = 0;
}

/**
 * @brief  更新正弦波输出
 */
void DAC0832_SineWave_Update(void)
{
  DAC0832_Write(sine_table[sine_index]);
  sine_index = (sine_index + 1) % SINE_POINTS;
}