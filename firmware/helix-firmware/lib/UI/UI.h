#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

namespace Theme
{
    // Theme-1 Slate
    // constexpr uint16_t BACKGROUND = ST77XX_BLACK;
    // constexpr uint16_t TEXT = 0xE73C;

    // constexpr uint16_t PRIMARY = 0x4ACF;
    // constexpr uint16_t SUCCESS = ST77XX_GREEN;
    // constexpr uint16_t WARNING = ST77XX_YELLOW;
    // constexpr uint16_t MUTED =  0x8C71;

    // Theme-2 Forest
    // constexpr uint16_t BACKGROUND = ST77XX_BLACK;
    // constexpr uint16_t TEXT = 0xE73C;

    // constexpr uint16_t PRIMARY = 0x5B87;
    // constexpr uint16_t SUCCESS = ST77XX_GREEN;
    // constexpr uint16_t WARNING = ST77XX_YELLOW;
    // constexpr uint16_t MUTED =  0x8C50;

    // Theme-3 Sunset
    // constexpr uint16_t BACKGROUND = ST77XX_BLACK;
    // constexpr uint16_t TEXT = 0xE73C;

    // constexpr uint16_t PRIMARY = 0xA986;
    // constexpr uint16_t SUCCESS = ST77XX_GREEN;
    // constexpr uint16_t WARNING = ST77XX_YELLOW;
    // constexpr uint16_t MUTED =  0x9491;

    // Theme-4 Mono
    constexpr uint16_t BACKGROUND = ST77XX_BLACK;
    constexpr uint16_t TEXT = 0xE73C;

    constexpr uint16_t PRIMARY = 0xC618;
    constexpr uint16_t SUCCESS = ST77XX_GREEN;
    constexpr uint16_t WARNING = ST77XX_YELLOW;
    constexpr uint16_t MUTED =  0x8410;
}

class UI
{
public:
    UI(Adafruit_ST7789 &display);

    void clear();

    void drawHeader(const char *title);

private:
    Adafruit_ST7789 &tft;
};