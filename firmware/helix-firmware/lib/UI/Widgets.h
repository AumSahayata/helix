#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

namespace Widgets
{
    void formatTime(
        uint32_t milliseconds,
        char *buffer);

    String fitText(
        Adafruit_ST7789 &tft,
        const char *text,
        int maxWidth);

    void drawProgressOutline(
        Adafruit_ST7789 &tft,
        int x,
        int y,
        int width,
        int height,
        uint16_t color);

    void drawProgressFill(
        Adafruit_ST7789 &tft,
        int x,
        int y,
        int width,
        int height,
        uint32_t value,
        uint32_t maxValue,
        uint16_t color,
        uint16_t background);
}