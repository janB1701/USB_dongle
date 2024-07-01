#!/usr/bin/env python3

import argparse
import serial
import random

command_help = {
"ping": "Send ping",
"flash": "Usage: flash <read/write> <filename>",
"led": "Usage: led <on/off/blink>",
"adc": "Generate random number from adc value",
"blink": "Usage: blink <duration_ms>",
"stop_blink": "Stop LED blink",
"exit": "Exit the CLI"
}

# Initialize serial communication
def init_serial(port):
    return serial.Serial(port, baudrate=115200, timeout=1)

# First call to dongle
def init (ser):
    init_command = f'ping_morning\r'
    ser.write (init_command.encode())
    response = ser.readline().strip()
    if response == b'dongle is alive':
        print ("Dongle is alive!!!!")
        return 1;
    else:
        print ("Dongle is not alive!!!!")
        return 0;

# Handle ping command
def handle_ping(ser):
    ser.write(b'ping\r')  # Make sure to end with a \r character
    response = ser.readline().strip()  # Use readline to capture the response
    if response == b'pong' or response == b'\x00pong':
        print("Dongle is alive")
    else:
        print("No response from dongle")

# flash read/write
def handle_flash(ser, action, filename):
    if action == 'read':
        ser.write(b'read_flash\r')
        print ("reading flash")
        data = ser.readline()
        if data == '' or data == b'\x00': # If there was a problem with reading usart msgs
            print ("reading again")
            ser.write(b'read_flash\r')
            data = ser.readline()
        #cleaned_data = data.replace(b'\x00', b'')
        data = data.replace (b'\xfe\xff', b'')
        cleaned_data = data.decode ("utf-8")
        if "NAN" in str(data):
            print ("There is no data in flash")
        else:
            print (cleaned_data)
            with open(filename, 'w') as f:
                f.write("%s" % cleaned_data)
    elif action == 'write':
        ser.write(b'write_flash\r')
        with open(filename, 'rb') as f:
            data = f.read()
        print ("ser write")
        ser.write(data)
    else:
        print("Unknown flash action")

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
    response = ser.readline().strip()
    try:
        adc_value = str(response)
        numeric_data = ''.join(filter(lambda x: x.isdigit(), adc_value))
        #print("ADC value:", numeric_data)
        random.seed (numeric_data)
        random_number = random.random()
        print ("Random number with ADC seed: " + str(int(random_number*1000)))
    except ValueError:
        print("Invalid ADC value received:", response)
    

# Blink LED for a specified duration
def handle_blink(ser, duration):
    command = f'blink {duration}\r'
    ser.write(command.encode())

# Stop blink LED
def handle_stop_blink(ser):
    command = f'stop_blink\r'
    ser.write(command.encode())

def show_help(command=None):
    if command:
        if command in command_help:
            print(f"{command}: {command_help[command]}")
        else:
            print(f"No help available for unknown command: {command}")
    else:
        print("Available commands:")
        for cmd, desc in command_help.items():
            print(f"  {cmd}: {desc}")

def main():
    parser = argparse.ArgumentParser(description="CLI tool for USB dongle")
    parser.add_argument("-p", "--port", default="/dev/ttyUSB0", help="Serial port")
    args = parser.parse_args()

    ser = init_serial(args.port)

    init_ok = init(ser); # Call dongle if it is alive



    while True:
        if init_ok == 0:
            break
        try:
            user_input = input("Enter command: ").strip().split()
            if not user_input:
                continue

            command = user_input[0]
            if command == "exit": # Exit the cli
                print("Exiting...")
                break
            elif command == "help":
                if len (user_input) == 2:
                    show_help(user_input[1])
                else:
                    show_help()
            elif command == "ping": # Send ping
                handle_ping(ser)
            elif command == "flash": # Placeholder for future implementations
                if len(user_input) < 3:
                    print("Usage: falsh <read/write> <filename>")
                    continue
                action = user_input[1]
                filename = user_input[2]
                handle_flash(ser, action, filename)
            elif command == "led":
                if len(user_input) < 2:
                    print("Usage: led <on/off/blink>")
                    continue
                action = user_input[1]
                handle_led(ser, action)
            elif command == "adc":
                handle_adc_read(ser)
            elif command == "stop_blink":
                handle_stop_blink(ser)
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
