import RPi.GPIO as GPIO
import serial
import time
from distance_test import get_distance

ser = serial.Serial('/dev/serial0', 115200, timeout=1)

try:
    while True:
        dist = get_distance()
        packet = f"<DIST:{dist:.1f}>\n"
        ser.write(packet.encode())
        print(f"Sent: {packet.strip()}")
        time.sleep(0.1)
        
except KeyboardInterrupt:
    GPIO.cleanup()
    ser.close()