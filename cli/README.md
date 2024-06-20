# USB Dongle CLI Tool

This Python script provides a command-line interface (CLI) for interacting with a USB dongle through a serial port. It supports various commands for pinging the device, controlling LEDs, reading/writing flash, and reading ADC for random number generator.

## Features

- **Ping Command**: Check if the dongle is responsive.
- **Flash Read/Write**: Read from and write to the flash.
- **LED Control**: Turn the LED on, off, or make it blink.
- **ADC Read**: Read analog-to-digital converter (ADC) values, generate random number.

## Requirements

- Python 3
- `pyserial` library

## Installation

Install the `pyserial` library.

## Usage

   ```bash
   python tool.py -p /dev/ttyACM0
   
   led on
   led off
   blink ms
   stop_blink
   ping
   write flash "file_name"
   read flash "file_name2"
