/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LED.h"
#include "sw_lcd.h"
#include "spi_lcd.h"
#include "i2c_oled.h"
#include "595driver.h"
#include "key.h"
#include "sw_keyboard.h"
#include "timer.h"
#include "tm1650.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define lab4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int key_current;
int key_history;
int s4_status = 0;
int s5_status = 0;
int clear_bits[5] = {0,0,0,0,0};
int lcd_flag = 0;
uint16_t ADC_value = 0;
float ADC_Value_float = 0;
char *name_key = "YangHaoTian Key3 pressed\n"; //27
char *number_key = "20221689 Key6 pressed\n"; //23
char ADC_str[64] ;
uint8_t uartRxBuffer[12];
uint8_t uartTxBuffer[128];
uint8_t ledStatus = 0;      // LED状态：0-关闭，1-打开，2-闪烁
uint8_t flashFrequency = 1; // 闪烁频率，默认1Hz
uint16_t rxIndex = 0; // 接收索引
uint8_t uartRxChar; // 单字符接收缓冲区


//extern int LED_status;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  LED_status = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
  HAL_UART_Receive_IT(&huart2, uartRxBuffer, sizeof(uartRxBuffer)); //开启串口接收中断
	//HAL_TIM_Base_Start_IT(&htim5);

	
	//��ʼ������,��Ļ��
	//OLED_Init();
	//OLED_CLS();
	//
	LCE_init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1); //led 
	//OLED_LAB_DISP_name();   //display name & number
  HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0); //led 
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 10);
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
    { 
    ADC_value = HAL_ADC_GetValue(&hadc1);   //获取AD值
    }
    ADC_Value_float = (float)ADC_value * 3.3 / 4096; //将AD值转换为电压值
    /* 下面是要求二*/
    if(scan_KeyBoard())
    {
      if (curr_key == '3')
      {
        HAL_UART_Transmit(&huart2, (uint8_t *)name_key, sizeof(name_key) * 30, 1000); //发送名字
      }
      if(curr_key == '6')
      {
        HAL_UART_Transmit(&huart2, (uint8_t *)number_key, sizeof(number_key) * 30, 1000); //发送学号
      }

    }
		
    LCD_Display_Lab(); //LCD display ADC value
        /*要求四*/
    

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim2){
    snprintf(ADC_str, sizeof(ADC_str), "ADC value: %.2fV\nADC value(12bit): %d\n", ADC_Value_float, ADC_value);
    HAL_UART_Transmit(&huart2, (uint8_t *)ADC_str, sizeof(ADC_str), 1000); //发送AD值
		                //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	}
	if(htim == &htim3){
        static uint8_t counter = 0;
        counter++;
        
        // 处理LED闪烁
        if(ledStatus == 2) {
            // 例如: 闪烁频率为2Hz时，每5次中断(10Hz/2Hz=5)翻转一次LED状态
            if(counter % (10/flashFrequency) == 0) {
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            }
				}
		}
	if(htim == &htim5){
	
		
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2) // 判断接收的串口是否为USART2
  {
    // 处理接收到的数据
        if(strstr((char*)uartRxBuffer, "open02LED"))
        {
            // 打开LED
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // 低电平点亮
            ledStatus = 1;
            
            // 回复确认信息
            sprintf((char*)uartTxBuffer, "LED D1已打开\r\n");
            HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "close02LED"))
        {
            // 关闭LED
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // 高电平熄灭
            ledStatus = 0;
            
            // 回复确认信息
            sprintf((char*)uartTxBuffer, "LED D1已关闭\r\n");
            HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "flicker02"))
        {
            // 解析闪烁频率
            char *freqStr = strstr((char*)uartRxBuffer, "flicker02") + 9;
            int freq = freqStr[0] - '0'; // 获取频率数字
            if(freq >= 1 && freq <= 9)
            {
                flashFrequency = freq;
                ledStatus = 2;
                // 回复确认信息
                sprintf((char*)uartTxBuffer, "LED D1正以%dHz频率闪烁\r\n", flashFrequency);
                HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
            }
            else
            {
                // 无效频率
                sprintf((char*)uartTxBuffer, "无效闪烁频率\r\n");
                HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
            }
        }
        else
        {
            // 回传收到的信息
            sprintf((char*)uartTxBuffer, "收到: %s\r\n", uartRxBuffer);
            HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        // 清空接收缓冲区并继续接收
        memset(uartRxBuffer, 0, sizeof(uartRxBuffer));
        HAL_UART_Receive_IT(&huart2, uartRxBuffer, sizeof(uartRxBuffer));
  }else{
    HAL_UART_Transmit(&huart2, (uint8_t *)"Error: Unknown UART instance\n", 30, 1000); // 错误处理
  }
	        //memset(uartRxBuffer, 0, sizeof(uartRxBuffer));

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
