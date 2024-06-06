# CLI Tool for USB Dongle

This repository contains simulation for the Command Line Interface (CLI) tool for interacting with a USB dongle device. The tool allows users to perform various operations such as EEPROM read/write, LED control, and ADC reading via virtual serial connection.

## Overview

The CLI tool consists of two main components:

1. **Main Script (`main.py`):**
   - This script serves as the entry point for interacting with the USB dongle device.
   - It provides a CLI interface for users to send commands and perform actions such as EEPROM read/write.

2. **Test Script (`test.py`):**
   - The test script simulates an ESP32 device that responds to commands sent over a serial connection.
   - It is used for testing the functionality of the CLI tool without requiring the actual hardware.

## Usage

To use the CLI tool:

1. Ensure that Python 3 is installed on your system.
2. Install `socat` if not already installed. This tool is used for creating virtual serial ports for testing.
3. Run `socat` to create virtual serial ports:
   ```bash
   socat -d -d pty,raw,echo=0 pty,raw,echo=0
