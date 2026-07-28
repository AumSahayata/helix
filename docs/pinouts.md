# Helix Pinout Reference

GPIO assignments for the ESP32-S3 used by Helix.

## GPIO Overview

| GPIO | Function | Interface |
|------|----------|-----------|
| GPIO4 | TFT Reset | Display |
| GPIO5 | TFT DC | Display |
| GPIO6 | TFT CS | SPI |
| GPIO10 | microSD CS | SPI |
| GPIO11 | SDA — TFT + microSD | SPI |
| GPIO12 | SCL — TFT + microSD | SPI |
| GPIO13 | MISO — microSD | SPI |
| GPIO16 | DAC BCK | I2S |
| GPIO17 | DAC LRCK | I2S |
| GPIO18 | DAC DIN | I2S |
| GPIO35 | UP Button | GPIO |
| GPIO36 | DOWN Button | GPIO |
| GPIO37 | SELECT Button | GPIO |
| GPIO38 | BACK Button | GPIO |

---

## ST7789 TFT Display

| ST7789 | ESP32-S3 |
|--------|-----------|
| VCC | 3V3 |
| GND | GND |
| SCL / SCK | GPIO12 |
| SDA / MOSI | GPIO11 |
| RST | GPIO4 |
| DC | GPIO5 |
| CS | GPIO6 |

---

## microSD

The TFT and microSD card share the same SPI bus.

| microSD | ESP32-S3 |
|---------|-----------|
| CS | GPIO10 |
| SCK | GPIO12 |
| MOSI | GPIO11 |
| MISO | GPIO13 |

Shared SPI lines:

    ESP32-S3
        |
        |-- GPIO12 SCK ----+---- TFT
        |                  |
        |                  +---- microSD
        |
        |-- GPIO11 MOSI ---+---- TFT
                           |
                           +---- microSD

Each device has its own Chip Select:

    TFT CS     -> GPIO6
    microSD CS -> GPIO10

---

## PCM5102A DAC

The DAC uses I2S independently from the SPI bus.

| PCM5102A | ESP32-S3 |
|----------|-----------|
| VIN | 5V |
| GND | GND |
| BCK | GPIO16 |
| LRCK | GPIO17 |
| DIN | GPIO18 |
| XSMT | 3V3 |
| FMT | NC |
| FLT | NC |
| DEMP | NC |

### Analog Output

| PCM5102A | Function |
|----------|----------|
| LOUT | Left audio |
| ROUT | Right audio |
| AGND | Audio ground |

The onboard 3.5 mm output can also be used.

---

## Navigation Buttons

All buttons use the ESP32's internal pull-up resistors (`INPUT_PULLUP`).

| Button | ESP32-S3 |
|--------|-----------|
| UP | GPIO35 |
| DOWN | GPIO36 |
| SELECT | GPIO37 |
| BACK | GPIO38 |

Each button is wired:

    GPIO ---- Button ---- GND

Released:

    GPIO = HIGH

Pressed:

    GPIO = LOW