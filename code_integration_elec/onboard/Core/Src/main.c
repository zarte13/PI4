/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ads1015.h"
#include "XbeeRF.h"
#include "RF_Reception.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADS1015_REG_CONV 0b00000001
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static ADS1015_Handle ads;
static float voltage;
static Xbee_message_receive receive_msg;
Xbee_Message send_msg;
volatile static bool new_rf_message = false;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  // todo : implémenter les séquences d'initialisation des différents modules
  HAL_TIM_Base_Start_IT(&htim1);
  XBeeRF_Init();
  send_msg.start_char = 0xFEEDCAFE;
  send_msg.end_char = 0xBEEFCACA;

  //XBee_StartReceive();
  //HAL_UART_Receive_IT(&huart1, &temp_byte, 1);

  HAL_GPIO_WritePin(SOLENOID_1_GPIO_Port,SOLENOID_1_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SOLENOID_2_GPIO_Port,SOLENOID_2_Pin,GPIO_PIN_RESET);
  /* USER CODE BEGIN 2 */
  ADS1015_Init(&ads, &hi2c1);

  ADS1015_StartConversion(&ads);

  send_msg.start_char = 0xCACAFEED;
  send_msg.end_char = 0xCAFEBEEF;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  GPIO_PinState valve1 = HAL_GPIO_ReadPin(SOLENOID_1_GPIO_Port, SOLENOID_1_Pin);
	  GPIO_PinState valve2 = HAL_GPIO_ReadPin(SOLENOID_2_GPIO_Port, SOLENOID_2_Pin);

	  switch ((valve1 << 1) | valve2)
	  {
	      case 0b00: send_msg.valves = 0;  break;
	      case 0b10: send_msg.valves = 1;  break;
	      case 0b01: send_msg.valves = 2;  break;
	      case 0b11: send_msg.valves = 3;  break;
	  }

	  //HAL_Delay(1000);
	  //printf("miaw\r\n");
	  if (ads.data_ready_flag) {
	      int16_t raw = ADS1015_GetResult(&ads);
	      ADS1015_PrepMsg(raw);
	  }

	  if (new_rf_message){
	      new_rf_message = false;

	      if ((receive_msg.start_receive == 0xFEEDCACA) &&
	          (receive_msg.fin_receive   == 0xBEEFCAFE)){
	    	  printf("caliss:%X",receive_msg.start_receive);

	          HAL_GPIO_WritePin(SOLENOID_1_GPIO_Port, SOLENOID_1_Pin,
	              receive_msg.message_receive_valve1 == 0xFFFFFFFF ? GPIO_PIN_SET : GPIO_PIN_RESET);

	          HAL_GPIO_WritePin(SOLENOID_2_GPIO_Port, SOLENOID_2_Pin,
	              receive_msg.message_receive_valve2 == 0xFFFFFFFF ? GPIO_PIN_SET : GPIO_PIN_RESET);

	          printf("valve1=%lu valve2=%lu\r\n",
	                 receive_msg.message_receive_valve1,
	                 receive_msg.message_receive_valve2);
	      }
	      else {
	          printf("Message invalide\r\n");
	          printf("Start : %X\n\r", receive_msg.start_receive);
	          printf("end : %X\n\r", receive_msg.fin_receive);
	      }
	  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == INTERRUPT_ADC_Pin) {
        ADS1015_EXTI_Callback(&ads);
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c == &hi2c1) {
        ADS1015_DMA_RxComplete(&ads);
    }
}

#include "usbd_cdc_if.h"
int _write(int file, char *ptr, int len)
{
    CDC_Transmit_HS((uint8_t *)ptr, len); // Remplacer HS par FS si nécessaire
    return len;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == USART1){
        memcpy(&receive_msg, rx_uart_buffer, sizeof(Xbee_message_receive));
        new_rf_message = true;
        HAL_UART_Receive_IT(&XBEE_RF_UART_HANDLE, rx_uart_buffer, sizeof(Xbee_message_receive));
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
    	XBeeRF_Task(); // LED toutes les 500ms
        // ou set un flag :
        // flag_500ms = 1;
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
#ifdef USE_FULL_ASSERT
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
