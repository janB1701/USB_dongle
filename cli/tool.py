#!/usr/bin/env python3

import argparse
import serial
import sys

# Initialize serial communication
def init_serial(port):
    return serial.Serial(port, baudrate=115200, timeout=1)

# First call to dongle
def init (ser):
    init_command = f'ping_morning\r'
    ser.write (init_command.encode())
    response = ser.readline().strip()
    if response == b'dongle is alive':
        print ("Dongle is alive")

# Handle ping command
def handle_ping(ser):
    ser.write(b'ping\r')  # Make sure to end with a \r character
    response = ser.readline().strip()  # Use readline to capture the response
    if response == b'pong':
        print("Dongle is alive")
    else:
        print("No response from dongle")

# EEPROM read/write, not implemented
def handle_eeprom(ser, action, filename):
    if action == 'read':
        ser.write(b'read_eeprom\r')
        data = ser.read(128) 
        with open(filename, 'wb') as f:
            f.write(data)
    elif action == 'write':
        ser.write(b'write_eeprom\r')
        with open(filename, 'rb') as f:
            data = f.read()
        ser.write(data)
    else:
        print("Unknown EEPROM action")

# Control LED
def handle_led(ser, action):
    if action == 'on':
        ser.write(b'LED on\r')
    elif action == 'off':
        ser.write(b'LED off\r')
    else:
        print("Unknown LED action")

# ADC read
def handle_adc_read(ser):
    ser.write(b'adc\r')
    value = ser.read(2)
    print("ADC Value:", int.from_bytes(value, 'big'))

# Blink LED for a specified duration
def handle_blink(ser, duration):
    command = f'blink {duration}\r'
    ser.write(command.encode())

# Stop blink LED
def handle_stop_blink(ser):
    command = f'stop_blink\r'
    ser.write(command.encode())

def main():
    parser = argparse.ArgumentParser(description="CLI tool for USB dongle")
    parser.add_argument("-p", "--port", default="/dev/ttyUSB0", help="Serial port")
    args = parser.parse_args()

    ser = init_serial(args.port)

    init(ser); # Call dongle if it is alive

    while True:
        try:
            user_input = input("Enter command: ").strip().split()
            if not user_input:
                continue

            command = user_input[0]
            if command == "exit": # Exit the cli
                print("Exiting...")
                break
            elif command == "ping": # Send ping
                handle_ping(ser)
            elif command == "eeprom": # Placeholder for future implementations
                if len(user_input) < 3:
                    print("Usage: eeprom <read/write> <filename>")
                    continue
                action = user_input[1]
                filename = user_input[2]
                handle_eeprom(ser, action, filename)
            elif command == "led":
                if len(user_input) < 2:
                    print("Usage: led <on/off/blink>")
                    continue
                action = user_input[1]
                handle_led(ser, action)
            elif command == "adc":
                handle_adc_read(ser)
            elif command == "stop_blink":
                handle_stop_blink(ser);
            elif command == "blink":
                if len(user_input) < 2:
                    print("Usage: blink <duration_ms>")
                    continue
                try:
                    duration = int(user_input[1])
                    handle_blink(ser, duration)
                except ValueError:
                    print("Invalid duration, must be an integer")
            else:
                print("Unknown command")
        except Exception as e:
            print(f"An error occurred: {e}")

    ser.close()

if __name__ == "__main__":
    main()
