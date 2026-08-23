# Helix Firmware Architecture

## Overview

Helix is built using a modular architecture where each module owns a single responsibility.

The goal is to keep hardware access, application logic, and user interface separated so new features can be added without rewriting existing code.

```
                +------------------+
                |    main.cpp      |
                |      Router      |
                +--------+---------+
                         |
       +-----------------+-----------------+
       |        |        |        |        |
       ▼        ▼        ▼        ▼        ▼
   HomeMenu   Music   Settings  Stopwatch Pomodoro

                 Shared Components
       +------------------------------------+
       | Input | UI | Widgets | Theme       |
       +------------------------------------+

                    Hardware
       +------------------------------------+
       | ESP32-S3 | ST7789 | SD | DAC | GPIO |
       +------------------------------------+
       ```

---

## Project Structure

```
lib/
│
├── Input/
│   ├── Input.h
│   └── Input.cpp
│
├── UI/
│   ├── UI.h
│   ├── UI.cpp
│   ├── HomeMenu.h
│   ├── HomeMenu.cpp
│   ├── Widgets.h
│   └── Widgets.cpp
│
├── Music/
│
├── Pomodoro/
│
├── Stopwatch/
│
src/
└── main.cpp
```

---

# Module Responsibilities

## main.cpp

Acts as the application router.

Responsibilities:

- Initialize hardware
- Create application objects
- Route InputEvents
- Handle navigation between screens

main.cpp should **not** contain application-specific logic.

---

## Input

Owns all button processing.

Responsibilities:

- Button polling
- Debouncing
- Hold detection
- InputEvent generation

Applications never read GPIO directly.

---

## UI

Provides reusable drawing functionality.

Responsibilities:

- Screen clearing
- Header drawing
- Theme colors
- Shared UI helpers

UI never owns application state.

---

## Widgets

Reusable UI components shared across applications.

Current widgets:

- Progress Bar
- Time Formatting
- Text Fitting
- Overlay Box
- Marquee Text

Future widgets:

- Battery Indicator
- Volume Indicator

---

## HomeMenu

Owns the launcher.

Responsibilities:

- Menu drawing
- Menu navigation
- Selection handling
- Returning HomeAction

HomeMenu does **not** perform navigation itself.

---

## Music

Owns the music player.

Responsibilities:

- Music library
- Playlist generation
- Playlist navigation
- Random playback
- MP3 playback
- Audio metadata
- Album artwork
- Volume control
- Progress display
- Music input handling

---

## Pomodoro

Owns all Pomodoro functionality.

Responsibilities:

- Timer logic
- Drawing
- Input handling

---

## Stopwatch

Owns stopwatch functionality.

Responsibilities:

- Timing
- Drawing
- Input handling

---

## Settings

Owns device configuration.

Responsibilities:

- Settings navigation
- Volume configuration
- Theme selection
- Persistent settings
- About page

---

# Input Flow

```
Button

↓

Input

↓

InputEvent

↓

Application

↓

Action
```

Example:

```
UP_PRESS

↓

Music::handleInput()

↓

Next Track
```

---

# Navigation Flow

```
Button

↓

HomeMenu

↓

HomeAction

↓

main.cpp

↓

Open Screen
```

Applications never change screens directly.

---

# Design Principles

- One responsibility per module.
- Keep `main.cpp` as a router.
- Avoid duplicate logic.
- Build reusable widgets.
- Applications own their own input.
- Prefer composition over inheritance.
- Refactor incrementally.
- Compile after every milestone.

---

# Future Architecture

Planned modules:

- Settings
- Calendar
- Password Manager
- Notes
- Storage
- Audio Engine
- Battery Manager

These will follow the same modular design used by the current applications.