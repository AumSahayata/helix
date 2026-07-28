# Helix Architecture

## Overview

Helix follows a layered architecture to keep hardware, business logic, and user interface independent.

```
Application
      │
      ▼
Screen Manager
      │
      ▼
Managers
├── AudioManager
├── DisplayManager
├── StorageManager
├── InputManager
└── PowerManager
      │
      ▼
Hardware Drivers
      │
      ▼
ESP32-S3
```

## Responsibilities

### Application

Owns the application lifecycle and coordinates the different managers.

### Screen Manager

Controls navigation between screens.

Examples:

* Home
* Music
* Settings
* Todo
* Now Playing

### Managers

Each manager owns one subsystem.

| Manager        | Responsibility              |
| -------------- | --------------------------- |
| AudioManager   | Music playback and DAC      |
| DisplayManager | Display drawing             |
| StorageManager | SD card and filesystem      |
| InputManager   | Button input                |
| PowerManager   | Battery, charging and sleep |

## Design Principles

* One responsibility per class.
* Hardware access only through managers.
* Screens never communicate directly with hardware.
* Keep `main.cpp` as small as possible.
