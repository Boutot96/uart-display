import smbus
import time

bus = smbus.SMBus(1)
ADS7830_ADDR = 0x4b

def read_channel(channel):
    command = 0x84 | (channel << 4)
    bus.write_byte(ADS7830_ADDR, command)
    return bus.read_byte(ADS7830_ADDR)

# try:
#     while True:
#         value = read_channel(0)
#         voltage = value * 3.3 / 255
#         print(f"Raw: {value} | Voltage: {voltage:.2f}V")
#         time.sleep(0.2)
# except KeyboardInterrupt:
#     pass