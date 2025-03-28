#include "DAC122S085.h"
extern SPI_HandleTypeDef hspi1;

/**
  * @brief  初始化DAC122S085
  * @param  hspi: SPI句柄
  * @retval None
  */
void DAC122S085_Init(SPI_HandleTypeDef *hspi)
{
  // 初始化CS引脚为高电平
  CS_H;
  HAL_Delay(1);
}

/**
  * @brief  设置DAC输出电压
  * @param  hspi: SPI句柄
  * @param  channel: DAC通道 (0 = A, 1 = B)
  * @param  value: 12位DAC值 (0-4095)
  * @retval None
  */
void DAC122S085_SetVoltage(SPI_HandleTypeDef *hspi, uint8_t channel, uint16_t value)
{
  uint16_t data = 0;
  
  // 限制输入值不超过12位
  value &= 0x0FFF;
  
  // 构建发送数据: 前4位控制位(A通道为0x1000, B通道为0x9000)
  if(channel == DAC_CHANNEL_A)
    data = 0x1000 | value;
  else // DAC_CHANNEL_B
    data = 0x9000 | value;
  
  // 发送数据
  CS_L;
  HAL_SPI_Transmit(hspi, (uint8_t*)&data, 2, HAL_MAX_DELAY);
  CS_H;
}
/********************************************
//功能描述：向DAC122S085写入数据，同时DA输出
//入口参数：ch：00：A通道，01：B通道，10：输出0，11：输出0
//          mod：00：写入数据，不刷新输出，01：写入数据，同时刷新输出
//          10：同时写入两个通道，同时刷新输出，11：休眠掉电，输出0
***********************************************/
void DAC122S085(unsigned char ch, unsigned char md, unsigned int daout) {
    // DAC122S085时序要求：SCL脉冲下降沿，极性0，低即：CPHA=2EDGE,CPOL=LOW
    uint16_t data;
    
    daout = daout & 0xfff;  // 只取12位数，因为是12位D/A
    
    data = (ch << 14) | (md << 12) | daout;  // 合成一个16位数，通道(2位)+模式(2位)+12位D/A的数
    
    CS_H;  // DAC122S085 SYNC低电平
    CS_L;  // DAC122S085 SYNC高电平
    // 查询阻塞式发送16位数据
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&data, 1, HAL_MAX_DELAY) != HAL_OK) {
        // 发送错误处理
        //Error_Handler();
    }
    
}