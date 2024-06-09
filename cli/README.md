# USB Dongle CLI Tool

This Python script provides a command-line interface (CLI) for interacting with a USB dongle through a serial port. It supports various commands for pinging the device, controlling LEDs, reading/writing EEPROM, and reading ADC values.

## Features

- **Ping Command**: Check if the dongle is responsive.
- **EEPROM Read/Write**: Read from and write to the EEPROM. (TO DO!)
- **LED Control**: Turn the LED on, off, or make it blink.
- **ADC Read**: Read analog-to-digital converter (ADC) values.
- **Blink Command**: Blink the LED for a specified duration.
- **Stop Blink Command**: Stop the blinking LED.

## Requirements

- Python 3
- `pyserial` library

## Installation

Install the `pyserial` library.

## Usage

   ```bash
   python usb_dongle_cli.py -p /dev/ttyACM0
   
   led on
   led off
   blink ms
   ping
