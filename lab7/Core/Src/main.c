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
char ADC_str[64] = "hello world\n"; //64;
uint8_t uartRxBuffer[10];
uint8_t uartTxBuffer[128];
uint8_t ledStatus = 0;      // LED状态：0-关闭，1-打开，2-闪烁
uint8_t flashFrequency = 1; // 闪烁频率，默认1Hz
uint16_t rxIndex = 0; // 接收索引
uint8_t uartRxChar; // 单字符接收缓冲区
int PWM_pulse = 100; // PWM占空比，初始值10%
int Moter_direction = 0; // 电机方向，0-正转，1-反转，2-停止

volatile uint32_t IC_Val1 = 0;          // 保存第一次捕获值
volatile uint32_t IC_Val2 = 0;          // 保存第二次捕获值
volatile uint32_t Difference = 0;       // 保存两次捕获的差值 (计数值)
volatile uint8_t Is_First_Captured = 0; // 状态标志，0表示等待第一次捕获，1表示等待第二次捕获
volatile float Frequency = 0.0f;        // 计算得到的频率 (Hz)
volatile float RPM = 0.0f;            // 计算得到的转速 (RPM)
const uint32_t Timer_ARR = 65535;     // TIM2的ARR值 (根据配置修改)
const float Timer_Clock_Frequency = 1000000.0f; // 定时器计数频率 (Hz), 根据PSC和时钟源计算得到
                                                // 例如 72MHz / (71+1) = 1MHz
const float Pulses_Per_Revolution = 10.0f; // 每转脉冲数 (根据实际传感器修改)
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3); //开启定时器中断
  HAL_TIM_Base_Start_IT(&htim2); //开启定时器中断
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); //开启输入捕获中断
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //开启PWM输出
  HAL_UART_Receive_IT(&huart2, uartRxBuffer, sizeof(uartRxBuffer)); //开启串口接收中断
	LCE_init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0); //led 
	OLED_Init();
	OLED_CLS();
	//OLED_LAB_DISP_name();   //display name & number
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(scan_KeyBoard())
    {
      if (curr_key == '0')
      {
        //正转
        Change_motor_direction(0);
        Moter_direction = 0;
      }
      if(curr_key == '4')
      {
        //反转
        Change_motor_direction(1);
        Moter_direction = 1;
      }
      if(curr_key == '8')
      {
        //停止
        Change_motor_direction(2);
        Moter_direction = 2;
      }
      if(curr_key == '5')
      {
        //加速
        Moteor_Speed_change(1);
      }
			 if(curr_key == '9')
      {
        //加速
        Moteor_Speed_change(1);
      }
      if (curr_key == '6')
      {
        //减速
        Moteor_Speed_change(2);
      }
			if (curr_key == 'A')
      {
        //减速
        Moteor_Speed_change(2);
      }
    }
    LCD_Display_Lab(); //LCD display ADC value
		OLED_LAB_DISP_name();        /*要求四*/
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
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim2){
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); //LED闪烁
    //HAL_UART_Transmit(&huart2, (uint8_t *)ADC_str, sizeof(ADC_str), 1000); //发送AD值
	}
	if(htim == &htim3){

		}
	if(htim == &htim5){
		
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2) // 判断接收的串口是否为USART2
  {
    // 处理接收到的数据
        if(strstr((char*)uartRxBuffer, "forwardMot"))
        {
            Change_motor_direction(0);
            
            // 回复确认信息
            sprintf((char*)uartTxBuffer, "Forward Motor\r\n");
            HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "backwardmt"))
        {
            Change_motor_direction(1);            
            // 回复确认信息
            sprintf((char*)uartTxBuffer, "Reverse motor\r\n");
            HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "sstopmotor"))
        {
            Change_motor_direction(2);
            sprintf((char*)uartTxBuffer, "stop motor\r\n");
            HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "speedupmtr"))
        {
          Moteor_Speed_change(1); 
          sprintf((char*)uartTxBuffer, "speed up motor\r\n");
          HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "speeddownm"))
        {
          Moteor_Speed_change(2); 
          sprintf((char*)uartTxBuffer, "speed down motor\r\n");
          HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
        }
        else if(strstr((char*)uartRxBuffer, "speedat"))
        {
            // 解析闪烁频率
            char *freqStr = strstr((char*)uartRxBuffer, "speedat") + 7; // 定位到"speedat"后面
            int freq = 0;
    
            // 使用sscanf函数解析数字
            if(sscanf(freqStr, "%d", &freq) == 1)
            {
              // 检查是否在有效范围内（例如1-999）
              if(freq >= 1 && freq <= 999)
              {
                // 应用设置
                flashFrequency = freq;
                __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, freq);
            // 回复确认信息
                sprintf((char*)uartTxBuffer, "NEW PWM%dHz频率闪烁\r\n", flashFrequency);
                HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
              }
              else
              {
              // 无效频率范围
              sprintf((char*)uartTxBuffer, "无效闪烁频率，请输入1-999之间的值\r\n");
              HAL_UART_Transmit(&huart2, uartTxBuffer, strlen((char*)uartTxBuffer), 100);
              }
            }
            else
            {
              // 格式错误
              sprintf((char*)uartTxBuffer, "格式错误，请输入speedat后跟数字\r\n");
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
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    // 确保是TIM2的中断源且是通道1触发的 (虽然回调函数是特定的，但检查htim->Instance是好习惯)
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      if (Is_First_Captured == 0) // 等待第一次捕获
      {
        // 读取第一次捕获值
        IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // 读取CCR1寄存器值
        Is_First_Captured = 1; // 标记已捕获第一次
      }
      else // 已捕获第一次，现在是第二次捕获
      {
        // 读取第二次捕获值
        IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
  
        // 计算差值，处理定时器溢出
        if (IC_Val2 >= IC_Val1)
        {
          Difference = IC_Val2 - IC_Val1;
        }
        else // 定时器溢出
        {
          // ARR是自动重载值 (例如 65535)
          // 定时器周期是 ARR+1 个计数
          Difference = (Timer_ARR - IC_Val1) + IC_Val2 + 1;
          // 或者写成 Difference = ((htim->Instance->ARR) - IC_Val1) + IC_Val2 + 1;
        }
  
        // 计算频率 (Hz)
        if (Difference != 0) // 避免除以零
        {
          // 注意: Timer_Clock_Frequency 需要根据你的实际配置计算
          // Timer_Clock_Frequency = HAL_RCC_GetPCLK1Freq() / (htim->Init.Prescaler + 1);
          // **重要**: 如果APB1预分频器不是1分频 (RCC_ClkInitStruct.APB1CLKDivider)，
          // APB1上的定时器时钟通常是PCLK1的2倍 (除非PCLK1等于AHB时钟)。
          // 请务必通过CubeMX的时钟配置界面或参考手册确认TIM2的精确时钟源频率。
          // 假设 Timer_Clock_Frequency 已正确计算并定义为常量或变量
  
          Frequency = Timer_Clock_Frequency / (float)Difference;
  
          // 计算RPM
          RPM = (Frequency / Pulses_Per_Revolution) * 60.0f;
        }
        else
        {
          // 差值为0，可能速度过快或错误，置零处理
          Frequency = 0.0f;
          RPM = 0.0f;
        }
  
        // 将当前的捕获值作为下一次计算的第一个值，准备下一次测量
        IC_Val1 = IC_Val2;
        // Is_First_Captured 保持为 1，持续测量
      }
    }
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
