/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "ili9341_gfx.h"
#include "XbeeRF.h"
#include "RF_Reception.h"
#include <string.h>




/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static volatile int valve_1 = 1;
static volatile int valve_2 = 1;
ili9341_t *_screen;
static Xbee_message_receive receive_msg;
uint8_t rx_msg_buf[sizeof(Xbee_message_receive)];
volatile bool got_data = false;
volatile bool update_display = false;
static uint8_t old_valves = 0xFF;  // 0xFF so it draws on first run
static float old_pression = -999.0f;  // impossible initial value
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim1);
  //XBeeRF_Init();
  //send_msg.start_char = 0xFEEDCAFE;
  //send_msg.end_char = 0xBEEFCACA;



  _screen = ili9341_new(
      &hspi1,
      Void_Display_Reset_GPIO_Port, Void_Display_Reset_Pin,
      TFT_CS_GPIO_Port, TFT_CS_Pin,
      TFT_DC_GPIO_Port, TFT_DC_Pin,
      isoLandscapeFlip,
      NULL, NULL,
      NULL, NULL,
      itsNotSupported,
      itnNormalized);

  ili9341_fill_screen(_screen, ILI9341_BLACK);

  // === Static layout (drawn once) ===

  // --- Top section: Pressure ---
  ili9341_text_attr_t title_attr = {&ili9341_font_16x26, ILI9341_WHITE, ILI9341_BLACK, 112, 10};
  ili9341_draw_string(_screen, title_attr, "Pression");

  // Pressure value placeholder (will update in loop if needed)


  // --- Horizontal separator ---
  ili9341_fill_rect(_screen, ILI9341_WHITE, 0, 85, 320, 2);

  // --- Vertical separator ---
  ili9341_fill_rect(_screen, ILI9341_WHITE, 159, 85, 2, 155);

  // --- Valve 1 label ---
  ili9341_text_attr_t v1_title = {&ili9341_font_11x18, ILI9341_WHITE, ILI9341_BLACK, 28, 95};
  ili9341_draw_string(_screen, v1_title, "Valve 1");

  // --- Valve 2 label ---
  ili9341_text_attr_t v2_title = {&ili9341_font_11x18, ILI9341_WHITE, ILI9341_BLACK, 190, 95};
  ili9341_draw_string(_screen, v2_title, "Valve 2");

  ili9341_fill_circle(_screen, ILI9341_YELLOW, 120, 104, 7);
  ili9341_fill_circle(_screen, ILI9341_YELLOW, 280, 104, 7);
  int prev_v1 = -1;
  int prev_v2 = -1;


  XBeeRF_Init();
  send_msg.start_char = 0xFEEDCACA;
  send_msg.end_char = 0xBEEFCAFE;
  send_msg.message_send_valve1 = 0X0;
  send_msg.message_send_valve2 = 0x0;
  HAL_UART_Receive_IT(&huart1, rx_msg_buf, sizeof(Xbee_message_receive));


  HAL_GPIO_WritePin(XBEE_N_RESET_GPIO_Port, XBEE_N_RESET_Pin, GPIO_PIN_SET);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	    int v1 = HAL_GPIO_ReadPin(VALVE_2_GPIO_Port, VALVE_2_Pin);
	    int v2 = HAL_GPIO_ReadPin(VALVE_1_GPIO_Port, VALVE_1_Pin);

	    for(volatile uint32_t i = 0; i < 10000; i++);

	    v1 &= HAL_GPIO_ReadPin(VALVE_2_GPIO_Port, VALVE_2_Pin);
	    v2 &= HAL_GPIO_ReadPin(VALVE_1_GPIO_Port, VALVE_1_Pin);
      valve_1 = v1;
      valve_2 = v2;

//      send_msg.valves = (v2 << 1) | v1;
//      XBeeRF_Task();




      int v1_changed = (v1 != prev_v1);
      int v2_changed = (v2 != prev_v2);


      if(v1_changed)
      {

          prev_v1 = v1;
          ili9341_color_t color = v1 ? ILI9341_GREEN : ILI9341_RED;

          // Clear ON/OFF zone
          ili9341_fill_rect(_screen, ILI9341_BLACK, 15, 130, 130, 95);

          // Draw border rectangle
          ili9341_draw_rect(_screen, color, 20, 135, 120, 85);
          ili9341_draw_rect(_screen, color, 21, 136, 118, 83);

          // Draw ON or OFF centered in box
          ili9341_text_attr_t v1_state = {&ili9341_font_16x26, color, ILI9341_BLACK, 0, 160};
          if(v1) {
              v1_state.origin_x = 56;
              ili9341_draw_string(_screen, v1_state, "ON");
              send_msg.message_send_valve1 = 0xFFFFFFFF;
          } else {
              v1_state.origin_x = 48;
              ili9341_draw_string(_screen, v1_state, "OFF");
              send_msg.message_send_valve1 = 0x00000000;
          }
      }

      if(v2_changed)
      {
          prev_v2 = v2;
          ili9341_color_t color = v2 ? ILI9341_GREEN : ILI9341_RED;

          // Clear ON/OFF zone
          ili9341_fill_rect(_screen, ILI9341_BLACK, 175, 130, 130, 95);

          // Draw border rectangle
          ili9341_draw_rect(_screen, color, 180, 135, 120, 85);
          ili9341_draw_rect(_screen, color, 181, 136, 118, 83);

          // Draw ON or OFF centered in box
          ili9341_text_attr_t v2_state = {&ili9341_font_16x26, color, ILI9341_BLACK, 0, 160};
          if(v2) {
              v2_state.origin_x = 216;
              ili9341_draw_string(_screen, v2_state, "ON");
              send_msg.message_send_valve2 = 0xFFFFFFFF;
          } else {
              v2_state.origin_x = 208;
              ili9341_draw_string(_screen, v2_state, "OFF");
              send_msg.message_send_valve2 = 0x00000000;
          }
      }


     // printf("%d\n\r", got_data);
      if (update_display)
      {
          update_display = false;
          switch (receive_msg.valves)
          {

              case 0x00:
            	 if(old_valves == 0x00)break;
                  ili9341_fill_circle(_screen, ILI9341_RED, 120, 104, 7);
                  ili9341_fill_circle(_screen, ILI9341_RED, 280, 104, 7);
                  old_valves = receive_msg.valves;
                  break;

              case 0x01:
            	  if(old_valves == 0x01)break;
                  ili9341_fill_circle(_screen, ILI9341_GREEN, 120, 104, 7);
                  ili9341_fill_circle(_screen, ILI9341_RED,   280, 104, 7);
                  old_valves = receive_msg.valves;
                  break;

              case 0x02:
            	  if(old_valves == 0x02)break;
                  ili9341_fill_circle(_screen, ILI9341_RED,   120, 104, 7);
                  ili9341_fill_circle(_screen, ILI9341_GREEN, 280, 104, 7);
                  old_valves = receive_msg.valves;
                  break;

              case 0x03:
            	  if(old_valves == 0x03)break;
                  ili9341_fill_circle(_screen, ILI9341_GREEN, 120, 104, 7);
                  ili9341_fill_circle(_screen, ILI9341_GREEN, 280, 104, 7);
                  old_valves = receive_msg.valves;
                  break;

              default:
                  ili9341_fill_circle(_screen, ILI9341_YELLOW, 120, 104, 7);
                  ili9341_fill_circle(_screen, ILI9341_YELLOW, 280, 104, 7);
                  break;
          }
      }
      if (got_data)
      {
          got_data = false;

          if (receive_msg.start_receive == 0xCACAFEED &&
              receive_msg.fin_receive   == 0xCAFEBEEF)
          {
              printf("=== Valid Message ===\r\n");
              printf("Start:    0x%08lX\r\n", (unsigned long)receive_msg.start_receive);
              int int_part = (int)receive_msg.pression;
              int dec_part = abs((int)(receive_msg.pression * 100) % 100);
              printf("Pression: %d.%02d\r\n", int_part, dec_part);
              printf("Valves:   0x%02X\r\n",  receive_msg.valves);
              printf("End:      0x%08lX\r\n", (unsigned long)receive_msg.fin_receive);

              // Clear both indicator circles first (black filled circle to erase)
              if (receive_msg.pression != old_pression)
              {
                  old_pression = receive_msg.pression;

                  char press_str[16];
                  int p_int = (int)receive_msg.pression;
                  int p_dec = abs((int)(receive_msg.pression * 100) % 100);
                  sprintf(press_str, "%d.%02d Pa", p_int, p_dec);

                  ili9341_fill_rect(_screen, ILI9341_BLACK, 0, 45, 320, 30);

                  int len = strlen(press_str);
                  int x_pos = (320 - (len * 16)) / 2;
                  ili9341_text_attr_t press_attr = {&ili9341_font_16x26, ILI9341_CYAN, ILI9341_BLACK, x_pos, 45};
                  ili9341_draw_string(_screen, press_attr, press_str);
              }
          }
          else
          {
              printf("=== Invalid Message ===\r\n");
              printf("Raw: ");
              for (uint32_t i = 0; i < sizeof(Xbee_message_receive); i++)
                  printf("%02X ", rx_msg_buf[i]);
              printf("\r\n");
          }
      }

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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        memcpy(&receive_msg, rx_msg_buf, sizeof(Xbee_message_receive));
        got_data = true;
        HAL_UART_Receive_IT(&huart1, rx_msg_buf, sizeof(Xbee_message_receive));
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM1)
    {
        XBeeRF_Task();
        update_display = true;
    }
}

#include "usbd_cdc_if.h"
int _write(int file, char *ptr, int len)
{

    CDC_Transmit_HS((uint8_t *)ptr, len);
    return len;
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
