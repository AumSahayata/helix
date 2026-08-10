#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

namespace Icons
{

    constexpr int16_t kIconSize = 8;
    constexpr int16_t kIconHeight = 16;
    constexpr int16_t kBarWidth = 3;
    constexpr int16_t kGap = 4;

    enum class Type : uint8_t
    {
        None,
        Previous,
        Play,
        Pause,
        Next,
        Random,
        Folder,
        Music,
        Battery,
        Bluetooth,
        Settings,
        Search,
        Back
    };

    void draw(
        Adafruit_ST7789 &,
        int16_t,
        int16_t,
        Type,
        uint16_t);
}