#!/usr/bin/env python3

import argparse
import serial
import sys

# Initialize serial communication
def init_serial(port):
    return serial.Serial(port, baudrate=115200, timeout=1)

# Handle ping command
def handle_ping(ser):
    ser.write(b'ping\n')  # Make sure to end with a newline character if using readline on the other end
    response = ser.readline().strip()  # Use readline to capture the response
    if response == b'pong':
        print("Dongle is alive")
    else:
        print("No response from dongle")

# EEPROM read/write
def handle_eeprom(ser, action, filename):
    if action == 'read':
        ser.write(b'read_eeprom\n')
        data = ser.read(128) 
        with open(filename, 'wb') as f:
            f.write(data)
    elif action == 'write':
        ser.write(b'write_eeprom\n')
        with open(filename, 'rb') as f:
            data = f.read()
        ser.write(data)
    else:
        print("Unknown EEPROM action")

# Control LED
def handle_led(ser, action):
    if action == 'on':
        ser.write(b'led_on\n')
    elif action == 'off':
        ser.write(b'led_off\n')
    elif action == 'blink':
        ser.write(b'led_blink\n')
    else:
        print("Unknown LED action")

    response = ser.readline().strip()
    print ("Recheived data: " + response.decode())

# ADC read (bonus)
def handle_adc_read(ser):
    ser.write(b'read_adc\n')
    value = ser.read(2)
    print("ADC Value:", int.from_bytes(value, 'big'))

def main():
    parser = argparse.ArgumentParser(description="CLI tool for USB dongle")
    parser.add_argument("-p", "--port", default="/dev/ttyUSB0", help="Serial port")
    args = parser.parse_args()

    ser = init_serial(args.port)

    while True:
        try:
            user_input = input("Enter command: ").strip().split()
            if not user_input:
                continue

            command = user_input[0]
            if command == "exit":
                print("Exiting...")
                break
            elif command == "ping":
                handle_ping(ser)
            elif command == "eeprom":
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
            else:
                print("Unknown commandddddd")
        except Exception as e:
            print(f"An error occurred: {e}")

    ser.close()

if __name__ == "__main__":
    main()