/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "flash.h"
#include "adc.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 1024 // Define RX_BUFFER_SIZE
//word length used for string compare in UART interrupt
#define ping_morning_diff 12
#define ping_diff 4
#define LED_on_diff 6
#define LED_off_diff 7
#define adc_diff 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//UART
uint8_t data_buffer[BUFFER_SIZE]; // data buffer
uint8_t recvd_data; // receive buffer
uint32_t count=0; // count how many bytes are received
uint32_t no_saved_chars = 0;

// flag variables used in UART interrupt
volatile uint8_t led_blinking = 0;
volatile uint32_t blink_counter = 0;
volatile uint32_t total_blink_counter = 0;
volatile uint8_t morning_ping = 0;
volatile uint8_t ping_call = 0;
volatile uint8_t led_on = 0;
volatile uint8_t adc_call = 0;
volatile uint8_t blink_ms_call = 0;
volatile uint8_t stop_blink = 0;
volatile uint8_t read_flash = 0;
volatile uint8_t data_arrived = 0;
uint8_t turn_off_led = 0;
volatile uint8_t flash_data = 0;
volatile uint8_t ready_to_write = 0;
char ptr[3];
char i[5];

//ADC1
uint16_t AD_RES = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void morning_led();
//void SendDataOverUART(uint8_t *data, uint32_t length);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



// Function used for setting initial call to STM (blink LED 2 times)
void morning_led() {
    led_blinking = 1;
    blink_counter = 0;
    total_blink_counter = 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint8_t pong_morning[] = "dongle is alive\r\n";
  uint8_t pong_msg[] = "pong\r\n";
  uint8_t read_fail[] = "NAN";
  int toggle = 0;
  char send_adc[10];
  uint32_t write_to_addr = 0x0801E010;

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
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  // Start UART reception in interrupt mode
  HAL_UART_Receive_IT(&huart3, &recvd_data, 1);

  // Start TIM6 in interrupt mode
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADC_Start_IT(&hadc1);
  HAL_StatusTypeDef HAL_FLASH_Unlock(void);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	if (data_arrived == 1){
		process_uart_command(data_buffer);
		data_arrived = 0;
		memset(data_buffer, 0, count);
		count = 0;
	}else{
		// used for Init fo dongle, it blinks 2 times and send message that it is alive
		if (morning_ping == 1){
			morning_ping = 0;
			morning_led();
			toggle = 100; //for Init part blinking is set to 100ms
			turn_off_led = 1;
			SendDataOverUART(pong_morning, sizeof(pong_morning));
		}
		// statements for "dongle alive - init" and also used for "blink xx ms" part of task
		if (led_blinking) {
		  // Check if it's time to toggle the LED
		  if (blink_counter >= toggle) { // this will toggle LED every toggle ms, timer is set so that it overflows every 1ms
			  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			  blink_counter = 0; // Reset the toggle counter
		  }

		  // Check if the total blink duration has been reached
		  if (total_blink_counter >= 500 && turn_off_led == 1) { // turn off LED after 500 for Init part of program, or keep it running
																 // if "blink xx ms" is activated, it will turn LED off after "stop_blinking" command
			  led_blinking = 0;
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  total_blink_counter = 0; // Reset the total blink counter
		  }
		}else if (led_on == 1) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		}else {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		}
		// if "ping" is received send "pong" back
		if (ping_call == 1) {
			SendDataOverUART(pong_msg, sizeof(pong_msg));
			ping_call = 0;
		}
		// send adc value back
		if (adc_call == 1) {
			adc_call = 0;
			// convert the integer value to a string
			sprintf(send_adc, "%d", AD_RES);
			SendDataOverUART((uint8_t*)send_adc, sizeof(send_adc));
		}
		// some code from blink led for some ms have been added to, just to shorten interrupt call :)...
		if (blink_ms_call == 1) {
			led_blinking = 1;
			blink_ms_call = 0;
			sscanf(i, "%d", &toggle);
			turn_off_led = 0;
		}
		if (ready_to_write == 1) {
			//process_serial_data();
			Flash_Write_Data(write_to_addr , (uint32_t *)data_buffer, count);
			memset(data_buffer, 0, count); // enpty the data buffer
			//Flash_Read_Data(write_to_addr, count);
			no_saved_chars = count;
			count = 0;
			//write_to_addr += 0xFF;
			ready_to_write = 0;
			flash_data = 0;
		}
		if (read_flash == 1 && no_saved_chars != 0) {
			Flash_Read_Data(write_to_addr, no_saved_chars);
			read_flash = 0;
		}else if (read_flash == 1){
			SendDataOverUART(read_fail, sizeof(read_fail));
			read_flash = 0;
		}
		HAL_ADC_Start_IT(&hadc1); // Start again with ADC1 interrupt
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_CSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
