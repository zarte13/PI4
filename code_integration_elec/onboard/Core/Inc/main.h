/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INTERRUPT_ADC_Pin GPIO_PIN_0
#define INTERRUPT_ADC_GPIO_Port GPIOC
#define INTERRUPT_ADC_EXTI_IRQn EXTI0_IRQn
#define XBEE_LED_Pin GPIO_PIN_4
#define XBEE_LED_GPIO_Port GPIOC
#define SOLENOID_1_Pin GPIO_PIN_0
#define SOLENOID_1_GPIO_Port GPIOB
#define SOLENOID_2_Pin GPIO_PIN_1
#define SOLENOID_2_GPIO_Port GPIOB
#define XBEE_STATUS_Pin GPIO_PIN_9
#define XBEE_STATUS_GPIO_Port GPIOC
#define XBEE_RQ_Pin GPIO_PIN_8
#define XBEE_RQ_GPIO_Port GPIOA
#define XBEE_UART_IN_Pin GPIO_PIN_9
#define XBEE_UART_IN_GPIO_Port GPIOA
#define XBEE_UART_OUT_Pin GPIO_PIN_10
#define XBEE_UART_OUT_GPIO_Port GPIOA
#define XBEE_N_RESET_Pin GPIO_PIN_10
#define XBEE_N_RESET_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
