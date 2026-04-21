# UART Distance Display

A two-device embedded system that reads distance from an HC-SR04 ultrasonic sensor on a Raspberry Pi 4B and transmits the data over UART to an Elecrow CrowPanel ESP32 2.1" round display.

## Hardware

- Raspberry Pi 4B (8GB)
- HC-SR04 Ultrasonic Distance Sensor
- Elecrow CrowPanel 2.1" HMI ESP32 Rotary Display (480x480)
- Jumper wires

## Architecture
The Pi reads the sensor via GPIO timing, formats the distance into a structured packet `<DIST:42.3>` and transmits at 115200 baud. The CrowPanel receives and parses the packet and renders the distance live on screen.

## Wiring

### HC-SR04 → Pi 4B
| HC-SR04 | Pi GPIO |
|---------|---------|
| VCC | Pin 2 (5V) |
| GND | Pin 6 (GND) |
| TRIG | Pin 16 (GPIO 23) |
| ECHO | Pin 18 (GPIO 24) |

### Pi 4B → CrowPanel UART
| Pi | CrowPanel |
|----|-----------|
| Pin 8 (TX) | RX |
| Pin 10 (RX) | TX |
| Pin 6 (GND) | GND |

## Software

### Pi Side
- Python 3
- RPi.GPIO
- pyserial (`pip install pyserial`)

Run:
```bash
python3 pi/uart_sender.py
```

### CrowPanel Side
- Arduino IDE with ESP32 core 2.0.14
- Libraries from Elecrow GitHub repo (see `crowpanel/uart_receiver/`)
- Flash `crowpanel/uart_receiver/uart_receiver.ino`

## What I Learned

- UART protocol design and packet framing
- Cross-device serial communication between Linux and an ESP32
- HC-SR04 GPIO timing and distance calculation
- ESP32 RGB display driver initialization
- Embedded systems project structure across multiple devices
