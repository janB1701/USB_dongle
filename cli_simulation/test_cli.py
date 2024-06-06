#!/usr/bin/env python3

import serial
import time

def simulate_esp32(port):
    ser = serial.Serial(port, baudrate=115200, timeout=1)
    print(f"Listening on {port}")

    while True:
        if ser.in_waiting > 0:
            command = ser.readline().decode().strip()
            print(f"Received command: {command}")
            if command == "ping":
                ser.write(b"pong\n")
            elif command.startswith("read_eeprom"):
                #print ("Usao read\n")
                ser.write(b"EEPROM data\n")
            elif command.startswith("write_eeprom"):
                #print ("usao write\n")
                ser.write(b"EEPROM write OK\n")
            elif command.startswith("led"):
                ser.write(b"LED action OK\n")
            elif command.startswith("adc"):
                ser.write(b"ADC value\n")
            else:
                ser.write(b"Unknown command\n")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: ./test_cli <port>")
    else:
        simulate_esp32(sys.argv[1])