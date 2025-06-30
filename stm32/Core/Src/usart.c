///* USER CODE BEGIN Header */
///**
// ******************************************************************************
// * @file    usart.c
// * @brief   This file provides code for the configuration
// *          of the USART instances.
// ******************************************************************************
// * @attention
// *
// * Copyright (c) 2024 STMicroelectronics.
// * All rights reserved.
// *
// * This software is licensed under terms that can be found in the LICENSE file
// * in the root directory of this software component.
// * If no LICENSE file comes with this software, it is provided AS-IS.
// *
// ******************************************************************************
// */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "usart.h"
//
//#include "main.h"
//
//#include <string.h>
//
///* USER CODE BEGIN 0 */
//
///* USER CODE END 0 */
//
//UART_HandleTypeDef huart3;
//
///* USART3 init function */
//
//void MX_USART3_UART_Init(void) {
//
//  /* USER CODE BEGIN USART3_Init 0 */
//
//  /* USER CODE END USART3_Init 0 */
//
//  /* USER CODE BEGIN USART3_Init 1 */
//
//  /* USER CODE END USART3_Init 1 */
//  huart3.Instance = USART3;
//  huart3.Init.BaudRate = 115200;
//  huart3.Init.WordLength = UART_WORDLENGTH_8B;
//  huart3.Init.StopBits = UART_STOPBITS_1;
//  huart3.Init.Parity = UART_PARITY_NONE;
//  huart3.Init.Mode = UART_MODE_TX_RX;
//  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
//  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
//  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//  if (HAL_UART_Init( & huart3) != HAL_OK) {
//    Error_Handler();
//  }
//  if (HAL_UARTEx_SetTxFifoThreshold( & huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
//    Error_Handler();
//  }
//  if (HAL_UARTEx_SetRxFifoThreshold( & huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
//    Error_Handler();
//  }
//  if (HAL_UARTEx_DisableFifoMode( & huart3) != HAL_OK) {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN USART3_Init 2 */
//
//  /* USER CODE END USART3_Init 2 */
//
//}
//
//void HAL_UART_MspInit(UART_HandleTypeDef * uartHandle) {
//
//  GPIO_InitTypeDef GPIO_InitStruct = {
//    0
//  };
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {
//    0
//  };
//  if (uartHandle -> Instance == USART3) {
//    /* USER CODE BEGIN USART3_MspInit 0 */
//
//    /* USER CODE END USART3_MspInit 0 */
//
//    /** Initializes the peripherals clock
//     */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
//    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
//    if (HAL_RCCEx_PeriphCLKConfig( & PeriphClkInitStruct) != HAL_OK) {
//      Error_Handler();
//    }
//
//    /* USART3 clock enable */
//    __HAL_RCC_USART3_CLK_ENABLE();
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**USART3 GPIO Configuration
//    PA3     ------> USART3_RX
//    PA4     ------> USART3_TX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF13_USART3;
//    HAL_GPIO_Init(GPIOA, & GPIO_InitStruct);
//
//    /* USART3 interrupt Init */
//    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART3_IRQn);
//    /* USER CODE BEGIN USART3_MspInit 1 */
//
//    /* USER CODE END USART3_MspInit 1 */
//  }
//}
//
//void HAL_UART_MspDeInit(UART_HandleTypeDef * uartHandle) {
//
//  if (uartHandle -> Instance == USART3) {
//    /* USER CODE BEGIN USART3_MspDeInit 0 */
//
//    /* USER CODE END USART3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USART3_CLK_DISABLE();
//
//    /**USART3 GPIO Configuration
//    PA3     ------> USART3_RX
//    PA4     ------> USART3_TX
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3 | GPIO_PIN_4);
//
//    /* USART3 interrupt Deinit */
//    HAL_NVIC_DisableIRQ(USART3_IRQn);
//    /* USER CODE BEGIN USART3_MspDeInit 1 */
//
//    /* USER CODE END USART3_MspDeInit 1 */
//  }
//}
//
///* USER CODE BEGIN 1 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
//  if (recvd_data == '\r') //when enter is pressed go to this condition
//  {
//    //process_uart_command(data_buffer);
//    if (flash_data == 0) {
//      //process_uart_command(data_buffer);
//      data_arrived = 1;
//      //memset(data_buffer, 0, count); // enpty the data buffer
//      //count = 0;
//    } else if (flash_data == 1) {
//      ready_to_write = 1;
//    }
//  } else {
//    data_buffer[count++] = recvd_data; // every time when interrput is happen, received 1 byte of data
//  }
//  HAL_UART_Receive_IT(&huart3, &recvd_data, 1); //start next data receive interrupt
//
//}
//
//// process commands from CLI
//void process_uart_command(uint8_t * cmd) {
//  if (strncmp((const char * ) cmd, "LED on", LED_on_diff) == 0) {
//    led_on = 1;
//  } else if (strncmp((const char * ) cmd, "LED off", LED_off_diff) == 0) {
//    led_on = 0;
//  } else if (strncmp((const char * ) cmd, "ping_morning", ping_morning_diff) == 0) {
//    morning_ping = 1;
//  } else if (strncmp((const char * ) cmd, "ping", ping_diff) == 0) {
//    ping_call = 1;
//  } else if (strncmp((const char * ) cmd, "adc", adc_diff) == 0) {
//    adc_call = 1;
//  } else if (strncmp((const char * ) cmd, "blink", 5) == 0) {
//    blink_ms_call = 1;
//    // first solution but for in interrupt is not good. but still I have choosed this option as you can enter delays like 10000
//    for (int j = 0; j < sizeof(ms); j++) {
//      if (cmd[6 + j] != '\0')
//        ms[j] = cmd[6 + j];
//      else
//        ms[j] = 0;
//    }
//
//    // there is a solution with strncpy but it is also too expensive for interrupt. only for delays up to 999
//    //strncpy(ptr, (const char*)data_buffer + 6, 3 - 1);
//  } else if (strncmp((const char * ) data_buffer, "stop_blink", 10) == 0) {
//    stop_blink = 1;
//    turn_off_led = 1;
//  } else if (strncmp((const char * ) data_buffer, "write_flash", 11) == 0) {
//    //memset(data_buffer, 0, count); // enpty the data buffer
//    //count = 0;
//    flash_data = 1;
//    //process_serial_data();
//  } else if (strncmp((const char * ) data_buffer, "read_flash", 10) == 0) {
//    read_flash = 1;
//  }
////    else if (strncmp((const char *)cmd, "get_sin", 7) == 0){
////	  sinus_measure = 1;
////  }
//}
//
//void SendDataOverUART(uint8_t * data, uint32_t length) {
//  HAL_UART_Transmit( & huart3, data, length, HAL_MAX_DELAY);
//}
///* USER CODE END 1 */




/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

#include "main.h"

#include <string.h>

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void) {

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init( & huart3) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold( & huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold( & huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode( & huart3) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef * uartHandle) {

  GPIO_InitTypeDef GPIO_InitStruct = {
    0
  };
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {
    0
  };
  if (uartHandle -> Instance == USART3) {
    /* USER CODE BEGIN USART3_MspInit 0 */

    /* USER CODE END USART3_MspInit 0 */

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig( & PeriphClkInitStruct) != HAL_OK) {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PA3     ------> USART3_RX
    PA4     ------> USART3_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_USART3;
    HAL_GPIO_Init(GPIOA, & GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    /* USER CODE BEGIN USART3_MspInit 1 */

    /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef * uartHandle) {

  if (uartHandle -> Instance == USART3) {
    /* USER CODE BEGIN USART3_MspDeInit 0 */

    /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PA3     ------> USART3_RX
    PA4     ------> USART3_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3 | GPIO_PIN_4);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
    /* USER CODE BEGIN USART3_MspDeInit 1 */

    /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
  if (recvd_data == '\r') //when enter is pressed go to this condition
  {
    data_buffer[count++] = '\0';
    //process_uart_command(data_buffer);
    if (flash_data == 0) {
      //process_uart_command(data_buffer);
      data_arrived = 1;
      //memset(data_buffer, 0, count); // enpty the data buffer
      //count = 0;
    } else if (flash_data == 1) {
      ready_to_write = 1;
    }
  } else {
    data_buffer[count++] = recvd_data; // every time when interrput is happen, received 1 byte of data
  }
  HAL_UART_Receive_IT(&huart3, &recvd_data, 1); //start next data receive interrupt

}

// process commands from CLI
void process_uart_command(uint8_t * cmd) {
  if (strncmp((const char * ) cmd, "LED on", LED_on_diff) == 0) {
    led_on = 1;
  } else if (strncmp((const char * ) cmd, "LED off", LED_off_diff) == 0) {
    led_on = 0;
  } else if (strncmp((const char * ) cmd, "ping_morning", ping_morning_diff) == 0) {
    morning_ping = 1;
  } else if (strncmp((const char * ) cmd, "ping", ping_diff) == 0) {
    ping_call = 1;
  } else if (strncmp((const char * ) cmd, "adc", adc_diff) == 0) {
    adc_call = 1;
  } else if (strncmp((const char * ) cmd, "blink", 5) == 0) {
    blink_ms_call = 1;
    // first solution but for in interrupt is not good. but still I have choosed this option as you can enter delays like 10000
    for (int j = 0; j < sizeof(ms); j++) {
      if (cmd[6 + j] != '\0')
        ms[j] = cmd[6 + j];
      else
        ms[j] = 0;
    }

    // there is a solution with strncpy but it is also too expensive for interrupt. only for delays up to 999
    //strncpy(ptr, (const char*)data_buffer + 6, 3 - 1);
  } else if (strncmp((const char * ) data_buffer, "stop_blink", 10) == 0) {
    stop_blink = 1;
    turn_off_led = 1;
  } else if (strncmp((const char * ) data_buffer, "write_flash", 11) == 0) {
    //memset(data_buffer, 0, count); // enpty the data buffer
    //count = 0;
    flash_data = 1;
    //process_serial_data();
  } else if (strncmp((const char * ) data_buffer, "read_flash", 10) == 0) {
    read_flash = 1;
  } else if (strncmp((const char * ) data_buffer, "get_sin", 7) == 0) {
	  sinus_measure = 1;
  }
}

void SendDataOverUART(uint8_t * data, uint32_t length) {
  HAL_UART_Transmit( & huart3, data, length, HAL_MAX_DELAY);
}
/* USER CODE END 1 */
