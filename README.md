# USB_dongle

## CLI and STM32 Project

This repository contains two main components: a Python CLI tool and STM32 firmware.

## CLI (Command-Line Interface)

The CLI tool, located in the `cli` directory, allows you to interact with a USB dongle via a serial port. It supports various commands such as pinging the dongle, controlling LEDs, reading/writing EEPROM (TO DO!), and reading ADC values.

### Usage

To use the CLI tool, navigate to the `cli` directory and run the Python script with the desired serial port:

## STM32 Firmware

The STM32 firmware, located in the stm32 directory, is designed to run on an STM32 microcontroller. It includes functionalities for handling UART communication, LED control, and other tasks as required for your application.

