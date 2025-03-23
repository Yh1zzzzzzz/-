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
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	//HAL_TIM_Base_Start_IT(&htim5);

	
	//��ʼ������,��Ļ��
	OLED_Init();
	OLED_CLS();
	//
	LCE_init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
	OLED_LAB_DISP_name();   //display name & number
  TM1650_Init();
  TM1650_SetBrightness(5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    /*要求一
    #ifdef lab1
      key_current = key_scan();
    if(key_current != 0 && key_current != key_history){
			for(int k = 0;k < 5;k++){
				clear_bits[k] = 0;
			}
      key_history = key_current;
    }
      if(key_history == 0){
				if(!clear_bits[0])
					OLED_CLS();
			  OLED_LAB_DISP_name();
				clear_bits[0] = 1;
      }else if(key_history == 1){
        if(!clear_bits[1])
					OLED_CLS();
        s2_pressed();
				clear_bits[1] = 1;
      }else if(key_history == 2){
        if(!clear_bits[2])
					OLED_CLS();
        s3_pressed();
				clear_bits[2] = 1;
      }else if(key_history == 3){
        if(!clear_bits[3])
					OLED_CLS();
        s4_status = 1;
        s5_status = 0;
        OLED_ShowStr(0,0,"S4 pressed",2); 
				clear_bits[3] = 1;
      }else if(key_history == 4){
        if(!clear_bits[4])
					OLED_CLS();
        s5_status = 1;
        s4_status = 0;
				clear_bits[4] = 1;
        OLED_ShowStr(0,0,"S5 pressed",2);
      }
    #endif
		*/
		 OLED_ShowStr(0,0,"timer",1);
		 OLED_ShowStr(0,3,"press 1-> start",1);
		 OLED_ShowStr(0,4,"press 2-> exit",1);
		 OLED_ShowStr(0,5,"press 3-> pause",1);
		 OLED_ShowStr(0,6,"press 4-> zero out",1);

    /* 下面是要求二*/
    scan_KeyBoard();
		if(lcd_flag)
			LAB2_request2();
		
    /*要求三*/
    uint8_t current_key = TM1650_ReadKey();
    if(current_key != 0){
      TM1650_DisplayKeyValue(current_key);
      HAL_Delay(50);
    }
    /*要求四*/
    #ifdef lab4
      
      timer(current_key);
    #endif

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
    if(s4_status == 1){
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }

	}
	if(htim == &htim3){
    if(s5_status == 1){
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
    if(time > 1000000000){
      time = 0;
    }
    if(pause != 1){
      time += 0.1;
    }
 	}
	if(htim == &htim5){
		time595 += 1;
		time595 = time595 % 8;
		
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
