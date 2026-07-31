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

class MarqueeText
{
public:
    void draw(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        int16_t width,
        const String &text,
        uint16_t color,
        uint16_t background);

private:
    int16_t offset = 0;

    enum class State
    {
        PauseStart,
        ScrollLeft,
        PauseEnd,
        ScrollRight
    };

    State state = State::PauseStart;

    uint32_t lastUpdate = 0;

    String currentText;
};