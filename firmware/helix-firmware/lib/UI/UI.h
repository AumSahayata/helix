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
    constexpr uint16_t MUTED = 0x8410;
}

class UI
{
public:
    UI(Adafruit_ST7789 &display);

    // Screen drawing
    void clear();
    void drawHeader(const char *title);
    void drawMenuItem(int index, const char *text, bool selected);

    // Layout helpers
    int16_t centerX(int16_t width) const;
    int16_t centerY(int16_t height) const;

    struct Position
    {
        int16_t x;
        int16_t y;
    };

    Position centerRect(int16_t width, int16_t height) const;

    struct VerticalLayout
    {
        explicit VerticalLayout(int16_t startY)
            : currentY(startY)
        {
        }

        int16_t currentY;
    };

    int16_t nextLine(
        VerticalLayout &layout,
        uint8_t textSize,
        int16_t gap = 0) const;

    int16_t nextBlock(
        VerticalLayout &layout,
        int16_t blockHeight,
        int16_t gap = 0) const;

    // Text helpers
    Position centeredTextPosition(
        const String &text,
        int16_t centerY,
        uint8_t size,
        bool centerVertically = true) const;

    void drawCenteredText(
        const String &text,
        int16_t centerY,
        uint8_t size,
        uint16_t color,
        bool centerVertically = true) const;

    int16_t textHeight(uint8_t size) const;

private:
    Adafruit_ST7789 &tft;
};