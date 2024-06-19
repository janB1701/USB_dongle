/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_flash.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define BUFFER_SIZE 1024 // Define RX_BUFFER_SIZE
//word length used for string compare in UART interrupt
#define ping_morning_diff 12
#define ping_diff 4
#define LED_on_diff 6
#define LED_off_diff 7
#define adc_diff 3

extern ADC_HandleTypeDef hadc1;

extern TIM_HandleTypeDef htim6;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

//UART
extern uint8_t data_buffer[BUFFER_SIZE]; // data buffer
extern uint8_t recvd_data; // receive buffer
extern uint32_t count; // count how many bytes are received

// flag variables used in UART interrupt
extern volatile uint8_t led_blinking;
extern volatile uint32_t blink_counter;
extern volatile uint32_t total_blink_counter;
extern volatile uint8_t morning_ping;
extern volatile uint8_t ping_call;
extern volatile uint8_t led_on;
extern volatile uint8_t adc_call;
extern volatile uint8_t blink_ms_call;
extern volatile uint8_t stop_blink;
extern volatile uint8_t read_next;
extern uint8_t turn_off_led;
extern volatile uint8_t flash_data;
extern volatile uint8_t ready_to_write;
extern volatile uint8_t read_flash;
extern volatile uint8_t data_arrived;
extern char ptr[3];
extern char i[5];

//ADC1
extern uint16_t AD_RES;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
