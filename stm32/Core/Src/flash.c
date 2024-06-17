/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    flash.c
  * @brief   This file provides code for the configuration
  *          of the flash instances.
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
#include "flash.h"
#include "stm32h5xx_hal_flash.h"
/* USER CODE BEGIN 0 */
uint8_t error = 0x07;
uint8_t ack = 0x06;
/* USER CODE END 0 */

/* FLASH init function */
void MX_FLASH_Init(void)
{

  /* USER CODE BEGIN FLASH_Init 0 */

  /* USER CODE END FLASH_Init 0 */

  /* USER CODE BEGIN FLASH_Init 1 */

  /* USER CODE END FLASH_Init 1 */
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_FLASH_Lock() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FLASH_Init 2 */

  /* USER CODE END FLASH_Init 2 */

}

/* USER CODE BEGIN 1 */
void erase_flash_page(uint32_t pageAddress) {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseInitStruct = {0};
    uint32_t sectorError;

    eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInitStruct.Sector = pageAddress; // space address to be erased
    eraseInitStruct.NbSectors = 1; // erase only 1 page

    if (HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError) != HAL_OK) {
    	HAL_UART_Transmit(&huart3, &error, sizeof(error), HAL_MAX_DELAY);
    }

    HAL_FLASH_Lock();
}


void write_to_flash(uint32_t startAddress, uint8_t *data, uint32_t length) {
    HAL_FLASH_Unlock(); // Unlock flash memory for writing

    uint64_t FData = 0x1A2B3C4D5E6F1234;		// Initialize the data to be written into the flash memory

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD,0x801fc00, FData);

    for (uint32_t i = 0; i < length; i += 2) { // half word (16 bits or 2 bytes) programming
        uint16_t halfWord = *(uint16_t *)(data + i);
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD_OTP, startAddress + i, halfWord) != HAL_OK) {
        	HAL_UART_Transmit(&huart3, &error, sizeof(error), HAL_MAX_DELAY);
        }
    }

    HAL_FLASH_Lock(); // Lock flash memory
}

void process_serial_data(void) {
    uint32_t startPageAddress = 0x0801FC00;
    uint32_t currentPageAddress = startPageAddress;
    uint32_t pageSize = 1024;

    while (1) {
        // Receive data
        //uint32_t bytesRead = HAL_UART_Receive(&huart3, data_buffer, BUFFER_SIZE, HAL_MAX_DELAY);
    	//uint32_t bytesRead = HAL_UART_Receive_IT(&huart3,&recvd_data,1);
        // Check if no data received
        if (count == 0) {
            break; // End of data
        }

        // Erase the current flash page
        erase_flash_page(currentPageAddress);

        // Write data to flash
       	write_to_flash(currentPageAddress, data_buffer, count);

        // Update the current page address
        currentPageAddress += pageSize;

        count = 0;
        flash_data = 0;
        //HAL_UART_Transmit(&huart3, &ack, sizeof(ack), HAL_MAX_DELAY);
    }
}

/* USER CODE END 1 */
