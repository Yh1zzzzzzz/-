#pragma once
#define SER_595_Pin GPIO_PIN_15
#define SER_595_GPIO_Port GPIOA
#define RCK_595_Pin GPIO_PIN_10
#define RCK_595_GPIO_Port GPIOC
#define SCK_595_Pin GPIO_PIN_11
#define SCK_595_GPIO_Port GPIOC

#ifndef __74HC595_H__
#define __74HC595_H__
 
 
#include "stm32f1xx_hal.h"

 
 
#define SHCP_GPIO_PORT      GPIOC             
#define SHCP_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define SHCP_GPIO_PIN	    GPIO_PIN_11	       
 
#define STCP_GPIO_PORT      GPIOC            
#define STCP_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define STCP_GPIO_PIN	    GPIO_PIN_10	        
 
#define DS_GPIO_PORT        GPIOA	              
#define DS_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define DS_GPIO_PIN	    GPIO_PIN_15
 
#define HC595_SHCP_Low()      HAL_GPIO_WritePin(SHCP_GPIO_PORT, SHCP_GPIO_PIN, 0);
#define HC595_SHCP_High()      HAL_GPIO_WritePin(SHCP_GPIO_PORT, SHCP_GPIO_PIN, 1); 
#define HC595_STCP_Low()        HAL_GPIO_WritePin(STCP_GPIO_PORT, STCP_GPIO_PIN, 0); 
#define HC595_STCP_High()    HAL_GPIO_WritePin(STCP_GPIO_PORT, STCP_GPIO_PIN, 1) ;
#define HC595_Data_Low()      HAL_GPIO_WritePin(DS_GPIO_PORT, DS_GPIO_PIN, 0);
#define HC595_Data_High()     HAL_GPIO_WritePin(DS_GPIO_PORT, DS_GPIO_PIN, 1);
 
 extern float time595 ;
extern int count[8];
void HC595_GPIO_Config(void);
void HC595_Send_Byte(unsigned char byte);
void HC595_CS(void);
void HC595_Send_Multi_Byte(unsigned char *data, unsigned short int len);
 
#define DIG_H 1
#define DIG_L  0
#define DP_H 1
#define DP_L 0

void LAB_595_display(void);
#endif