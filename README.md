# UART Distance & Voltage Display

A two-device embedded system that reads distance from an HC-SR04 ultrasonic sensor and analog voltage from a potentiometer via an ADS7830 I2C ADC on a Raspberry Pi 4B, and transmits both over UART to an Elecrow CrowPanel ESP32 2.1" round display.

## Hardware

- Raspberry Pi 4B (8GB)
- HC-SR04 Ultrasonic Distance Sensor
- ADS7830 I2C ADC (8-channel, 8-bit)
- 10k Potentiometer
- Elecrow CrowPanel 2.1" HMI ESP32 Rotary Display (480x480)
- Jumper wires

## Architecture

    HC-SR04 --GPIO--|
                    |--> Pi 4B --> UART --> CrowPanel ESP32 --> Round Display
    ADS7830 --I2C--|

The Pi reads both sensors and sends two structured UART packets at 115200 baud. The CrowPanel parses both packet types and renders them as live labels on screen using LVGL.

## Wiring

### HC-SR04 → Pi 4B
| HC-SR04 | Pi GPIO |
|---------|---------|
| VCC | Pin 2 (5V) |
| GND | Pin 6 (GND) |
| TRIG | Pin 16 (GPIO 23) |
| ECHO | Pin 18 (GPIO 24) |

### ADS7830 → Pi 4B (I2C)
| ADS7830 | Pi GPIO |
|---------|---------|
| VCC | Pin 1 (3.3V) |
| GND | Pin 9 (GND) |
| SDA | Pin 3 (GPIO 2) |
| SCL | Pin 5 (GPIO 3) |
| CH0 | Potentiometer wiper |

### Pi 4B → CrowPanel UART
| Pi | CrowPanel |
|----|-----------|
| Pin 8 (TX) | RX |
| Pin 10 (RX) | TX |
| Pin 6 (GND) | GND |

## UART Protocol

Two packet types are sent at ~10Hz:
- `<DIST:42.3>` — distance in cm from HC-SR04
- `<VOLT:1.65>` — voltage from potentiometer via ADS7830

Packets use angle bracket framing with a colon-separated key:value format, making the protocol easily extensible for additional sensors.

## Software

### Pi Side
- Python 3
- RPi.GPIO
- pyserial
- smbus

```bash
pip install pyserial smbus
python3 pi/uart_sender.py
```

### CrowPanel Side

Built with PlatformIO (VS Code). Open `crowpanel/uart_receiver/` as a PlatformIO project — all dependencies are pinned in `platformio.ini` and will be downloaded automatically.

Flash with `Ctrl+Alt+U` or `platformio run --target upload`.

Required ESP32 core: espressif32 @ 6.4.0 (Arduino core 2.0.14 equivalent).
## What I Learned

- UART protocol design and packet framing across two devices
- I2C ADC communication and analog sensor reading
- Cross-device serial communication between Linux and an ESP32
- HC-SR04 GPIO timing and distance calculation
- ESP32 RGB display driver initialization (ST7701 + Arduino GFX)
- LVGL integration for embedded GUI rendering
- Library version management for embedded projects
- PlatformIO project configuration and dependency pinning