/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"
#include "main.h"
#include <stdio.h>
#include "flash.h"

/* USER CODE BEGIN 0 */





/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
    PeriphClkInitStruct.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_CSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_INP0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_INP0
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//	if (adc_call == 1){
//	    //Read & Update The ADC Result
//	    AD_RES = HAL_ADC_GetValue(&hadc1);
//	    HAL_ADC_Stop_IT(&hadc1); // Disable ADC interrupt
//	}
	if (hadc->Instance == ADC1) {
        if (adc_index < N) {
            adc_buffer[adc_index++] = (uint32_t) HAL_ADC_GetValue(&hadc1);
        }

        if (adc_index >= N) {
            adc_index = 0;
            adc_buffer_full = 1;
            HAL_ADC_Stop_IT(&hadc1);  // Zaustavi ADC dok se obrada ne završi
        } else {
            HAL_ADC_Start_IT(&hadc1);  // Ponovno pokreni ADC
        }
    }
	HAL_ADC_Stop_IT(&hadc1);
}

uint16_t Read_ADC_Once(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop_IT(&hadc1);
    return value;
}

void ProcessSineBuffer(void) {
    float voltages[N];
    float sum = 0, vmax = 0, vmin = VREF;

    for (int i = 0; i < N; i++) {
        voltages[i] = ((float)adc_buffer[i]) * VREF / ADC_MAX;
    	//voltages[i] = Read_ADC_Once();
        sum += voltages[i];
        if (voltages[i] > vmax) vmax = voltages[i];
        if (voltages[i] < vmin) vmin = voltages[i];
        //HAL_Delay(100);
    }

    float avg = sum / N;
    float amplitude = (vmax - vmin) / 2.0f;

    char msg[100];
    int len = snprintf(msg, sizeof(msg), "DC offset: %.2f V, Amplitude: %.2f V\r\n", avg, amplitude);
    if (sinus_measure == 1){
    	HAL_UART_Transmit(&huart3, (uint8_t*)msg, len, HAL_MAX_DELAY);
    	Flash_Write_Data_ADC(write_to_addr, (uint32_t *) adc_buffer, 200);
    	flash_adc_write = 1;
    	saved_chars_cnt = 200;
    }
}

//void measure_sinus(void) {
//    const int num_samples = 100;
//    char buffer[32];
//    for (int i = 0; i < num_samples; i++) {
//        HAL_ADC_Start(&hadc1);
//        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//        uint16_t value = HAL_ADC_GetValue(&hadc1);
//
//        // Pretvori ADC vrijednost u string i pošalji preko USART
//        int len = snprintf(buffer, sizeof(buffer), "Sample %d: %d\r\n", i, value);
//        HAL_UART_Transmit(&huart3, (uint8_t*)buffer, len, HAL_MAX_DELAY);
//
//        HAL_Delay(10); // sampling period, ovdje 100Hz (10ms delay)
//    }
//
//    sinus_measure = 0;
//}
/* USER CODE END 1 */
