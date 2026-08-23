# Development Journal

## 2026-07-21

### Repository Created

* Created the Helix GitHub repository.
* Selected ESP32-S3 as the main controller.
* Chose PlatformIO with the Arduino framework.
* Ordered the initial hardware.

### Current Hardware

* ESP32-S3 DevKit
* ST7789 Display
* PCM5102A DAC
* TP4056 Charger
* 2000mAh Li-Po Battery

### Next Goal

Bring up the ESP32 and verify the development environment.

---

## 2026-07-29

* Received all the Hardware.

### Update

* Tested all the parts.
* Created Stopwatch and Pomodoro timer.


### Next Goal

Create better UI and try playing music over bluetooth

---

## 2026-08-01 to 2026-08-09

* Worked on UI.
* Created apps like music player, stopwatch, pomodoro and settings.
* Created Widgets for reusablility.


### Next Goal

Create try playing music over bluetooth, save settigns on the board. 

---

## 2026-08-10

### v0.1.0-alpha

* Tagged the first Helix firmware release: `v0.1.0-alpha`.
* Established the initial working firmware baseline.
* Completed the first usable hardware and UI foundation.
* Kept debug functionality in the release as part of the early alpha stage.

### Next Goal

Continue improving the UI and expand the music player and settings functionality.

---

## 2026-08-23

### v0.2.0-alpha

* Completed a major UI and firmware foundation milestone.
* Added Settings application.
* Added runtime theme system with persistent theme selection.
* Added About page.
* Improved volume settings and synchronization with the audio player.
* Added Play Random to the music library.
* Added music and random icons.
* Implemented partial UI redraws to reduce screen flicker.
* Added metadata-driven player UI updates.
* Fixed library navigation crash caused by invalid scroll offsets.
* Improved modular UI architecture and reusable helpers.

### Next Goal

Focus on further device-level features.