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

#include "string.h"

#include "stdio.h"

#include "main.h"

#include "usart.h"

/* USER CODE BEGIN 0 */
uint8_t error = 0x07;
uint8_t ack = 0x06;

#define FLASHWORD 16
/* USER CODE END 0 */

/* FLASH init function */
void MX_FLASH_Init(void) {

  /* USER CODE BEGIN FLASH_Init 0 */

  /* USER CODE END FLASH_Init 0 */

  /* USER CODE BEGIN FLASH_Init 1 */

  /* USER CODE END FLASH_Init 1 */
  if (HAL_FLASH_Unlock() != HAL_OK) {
    Error_Handler();
  }
  if (HAL_FLASH_Lock() != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN FLASH_Init 2 */

  /* USER CODE END FLASH_Init 2 */

}

/* USER CODE BEGIN 1 */

// There are 2 BANKS available for H745, BANK 1 (0x0800 0000 - 0x080F FFFF) and BANK 2 (0x0810 0000 - 0x080F FFFF)
// Both of them have Sectors 0 to 7.
// We will define the sectors in normal way (like Defined below), and later the BANK will be taken care by the HAL

static uint32_t GetSector(uint32_t Address) {
  uint32_t sector = 0;

  /* BANK 1 */
  if ((Address >= 0x08000000) && (Address <= 0x08001FFF)) {
    sector = FLASH_SECTOR_0;
  } else if ((Address >= 0x08002000) && (Address <= 0x08003FFF)) {
    sector = FLASH_SECTOR_1;
  } else if ((Address >= 0x08004000) && (Address <= 0x08005FFF)) {
    sector = FLASH_SECTOR_2;
  } else if ((Address >= 0x08006000) && (Address <= 0x08007FFF)) {
    sector = FLASH_SECTOR_3;
  } else if ((Address >= 0x08008000) && (Address <= 0x08009FFF)) {
    sector = FLASH_SECTOR_4;
  } else if ((Address >= 0x0800A000) && (Address <= 0x0800BFFF)) {
    sector = FLASH_SECTOR_5;
  } else if ((Address >= 0x0800C000) && (Address <= 0x0800DFFF)) {
    sector = FLASH_SECTOR_6;
  } else if ((Address >= 0x0800E000) && (Address <= 0x0800FFFF)) {
    sector = FLASH_SECTOR_7;
  }

  /* BANK 2 */
  else if ((Address >= 0x08010000) && (Address <= 0x08011FFF)) {
    sector = FLASH_SECTOR_0;
  } else if ((Address >= 0x08012000) && (Address <= 0x08013FFF)) {
    sector = FLASH_SECTOR_1;
  } else if ((Address >= 0x08014000) && (Address <= 0x08015FFF)) {
    sector = FLASH_SECTOR_2;
  } else if ((Address >= 0x08016000) && (Address <= 0x08017FFF)) {
    sector = FLASH_SECTOR_3;
  } else if ((Address >= 0x08018000) && (Address <= 0x08019FFF)) {
    sector = FLASH_SECTOR_4;
  } else if ((Address >= 0x0801A000) && (Address <= 0x0801BFFF)) {
    sector = FLASH_SECTOR_5;
  } else if ((Address >= 0x0801C000) && (Address <= 0x0801DFFF)) {
    sector = FLASH_SECTOR_6;
  } else if ((Address >= 0x0801E000) && (Address <= 0x0801FFFF)) {
    sector = FLASH_SECTOR_7;
  }

  return sector;
}

/* Some Controllers like STM32H7Ax have 128 sectors. It's not possible to write each one of them here.
   You can come up with easier ways to set the sector numbers. FOR EXAMPLE

static uint32_t GetSector(uint32_t Address)
{
  uint16_t address = Address-0x08000000;
  int mentissa = address/8192;  // Each Sector is 8 KB

  return mentissa;
}

*/

/* The DATA to be written here MUST be according to the List Shown Below

For EXAMPLE:- For H74x/5x, a single data must be 8 numbers of 32 bits word
If you try to write a single 32 bit word, it will automatically write 0's for the rest 7

*          - 256 bits for STM32H74x/5X devices (8x 32bits words)
*          - 128 bits for STM32H7Ax/BX devices (4x 32bits words)
*          - 256 bits for STM32H72x/3X devices (8x 32bits words)
*
*/

uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t * data, uint16_t numberofwords) {

  static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t SECTORError;
  int sofar = 0;
  int break_writing = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Erase the user Flash area */

  /* Get the number of sector to erase from 1st sector */

  uint32_t StartSector = GetSector(StartSectorAddress);
  //uint32_t StartSector = FLASH_SECTOR_7;
  uint32_t EndSectorAddress = StartSectorAddress + numberofwords * 4;
  //uint32_t EndSectorAddress = 0x0801FFFF;
  uint32_t EndSector = GetSector(EndSectorAddress);
  //uint32_t EndSector = FLASH_SECTOR_7;

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  //EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = StartSector;

  // The the proper BANK to erase the Sector
  if (StartSectorAddress < 0x08010000)
    EraseInitStruct.Banks = FLASH_BANK_1;
  else EraseInitStruct.Banks = FLASH_BANK_2;

  EraseInitStruct.NbSectors = (EndSector - StartSector) + 1;

  if (HAL_FLASHEx_Erase( & EraseInitStruct, & SECTORError) != HAL_OK) {
    return HAL_FLASH_GetError();
  }

  /* Program the user Flash area 8 WORDS at a time
   * (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
  //ne moze break_writing bit = 16 na pocetku, treba popravit
  while (break_writing < numberofwords) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, StartSectorAddress, (uint32_t) & data[sofar]) == HAL_OK) {
      StartSectorAddress += 0x10;
      sofar += 4;
      break_writing += 16;
    } else {
      /* Error occurred while writing data in Flash memory*/
      return HAL_FLASH_GetError();
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
   to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return 0;
}

void Flash_Read_Data(uint32_t StartSectorAddress, uint16_t numberofwords) {
  uint8_t data;
  while (1) {
    data = * (__IO uint8_t * ) StartSectorAddress;
    SendDataOverUART( & data, sizeof(data));
    StartSectorAddress += 0x01;
    data++;
    if (!(numberofwords--)) break;
  }

}
/* USER CODE END 1 */
