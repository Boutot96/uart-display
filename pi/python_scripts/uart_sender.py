import serial
import time
from distance_test import get_distance
from adc_test import read_channel

ser = serial.Serial('/dev/serial0', 115200, timeout=1)

try:
    while True:
        dist = get_distance()
        packet = f"<DIST:{dist:.1f}>\n"
        ser.write(packet.encode())
        print(f"Sent: {packet.strip()}")
        
        value = read_channel(0)
        voltage = value * 3.3 / 255
        packet = f"<VOLT:{voltage:.2f}>\n"
        ser.write(packet.encode())
        print(f"Sent: {packet.strip()}")
        time.sleep(0.1)
        
except KeyboardInterrupt:
    GPIO.cleanup()
    ser.close()