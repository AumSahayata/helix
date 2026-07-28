# Helix

> A modular handheld companion built on the ESP32-S3.

Helix is an open-source handheld device built from scratch around the ESP32-S3.

The project explores embedded systems, hardware integration, user interface design, connectivity, and firmware architecture by building a practical device with physical controls and a custom UI.

Rather than replacing a smartphone, Helix is designed as a dedicated companion for quick interactions with media, productivity tools, and connected devices.

## Current Capabilities

- ✅ ESP32-S3 firmware running with PlatformIO
- ✅ ST7789 display
- ✅ Physical button input
- 🚧 Menu and UI system
- 🚧 Hardware integration

## Planned Features

- Bluetooth media controller
- Calendar
- To-do list
- Pomodoro timer
- Stopwatch
- More to come ...

## Hardware

Helix currently uses:

- ESP32-S3
- ST7789 TFT display
- PCM5102A I2S DAC
- microSD storage
- Physical navigation buttons
- Rechargeable battery system

Detailed wiring and GPIO assignments are documented in [PINOUTS.md](docs/PINOUTS.md).

## Project Goals

Helix is being built primarily to explore and learn:

- Embedded C/C++
- ESP32 development
- Bluetooth
- Embedded UI design
- Storage and file systems
- Audio hardware
- Power management
- Hardware prototyping
- Firmware architecture

## Development

Firmware is built using PlatformIO with the Arduino framework.

The project is being developed incrementally, with individual hardware and software components tested before integration.