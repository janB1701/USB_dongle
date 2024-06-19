# STM32 USB Dongle Project

## Overview

This project involves creating a USB dongle using an STM32 microcontroller with several functions, controlled via a Linux command-line tool (CLI). Commands are sent to the dongle over a serial interface, which executes them and returns the results.

## Dongle Features

- **Ping:** Check if the dongle is alive.
- **Flash Read/Write:** Access Flash memory.
- **LED Control:** Turn the LED on/off.
- **ADC Reading:** Read analog-to-digital converter values.

## Implementation Details

### Hardware

- **Microcontroller:** STM32H503
- **Peripherals:** UART for serial communication, GPIO for LED control, ADC for analog readings

### Firmware

- **Language:** C
- **Libraries:** STM32 HAL libraries
- **IDE:** STM32CubeIDE

### Key Files

- **flash.c:** Handles flash read/write operations.
- **usart.c:** Configures UART communication and processes incoming commands.
