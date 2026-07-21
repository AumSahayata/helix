# Helix Hardware - Parts List

This document lists the hardware used in the Helix project.

---

# Core Components

| Component       | Model                                     | Notes                                                |
| --------------- | ----------------------------------------- | ---------------------------------------------------- |
| Microcontroller | ESP32-S3-WROOM-1 N16R8 DevKit             | 16 MB Flash, 8 MB PSRAM, onboard microSD card reader |
| Display         | 2.0" ST7789 IPS TFT Display (240×320 SPI) | Main user interface                                  |
| Audio DAC       | PCM5102A I2S DAC Module                   | Integrated 3.5 mm stereo output                      |
| Battery         | 3.7V 2000mAh Li-Po                        | Primary power source                                 |
| Charging Module | TP4056 USB-C Charger (DW01 Protection)    | Battery charging and protection                      |
| Buttons         | Tactile Push Buttons               | Navigation and playback controls                     |

---

# Storage

| Component    | Model          | Notes                                        |
| ------------ | -------------- | -------------------------------------------- |
| microSD Card | 32 GB | Stores music, settings, and application data |

---

# Power

| Component         | Notes                    |
| ----------------- | ------------------------ |
| USB Type-C Cable  | Programming and charging |
| Mini Slide Switch | Main power switch        |

---

# Prototyping

| Component             | Notes                         |
| --------------------- | ----------------------------- |
| Breadboard            | Hardware prototyping          |
| Jumper Wires          | Breadboard connections        |
| Silicone Hook-up Wire | Final internal wiring         |

---

# Enclosure

| Component             | Notes                         |
| --------------------- | ----------------------------- |
| Acrylic Sheets        | First enclosure prototype     |
| M2 Screws & Standoffs | Mount PCBs and acrylic layers |


---

# Software Stack

* C++17
* PlatformIO
* VS Code
* Arduino Framework
* Git
* GitHub

---

# Notes

* Prototype on a breadboard before soldering.
* Test each module independently before integrating.
* Record any hardware changes in this file as the project evolves.
