#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "Icons.h"
#include "Theme.h"

struct ControlHint;
enum class ControlHintsPosition;

class UI
{
public:
    UI(Adafruit_ST7789 &display);

    // Screen drawing
    void clear();
    void drawHeader(const char *title);
    void drawMenuItem(
        int index,
        const String &text,
        bool selected,
        Icons::Type icon = Icons::Type::None);

    void drawControlHints(
        const ControlHint *hints,
        size_t count,
        ControlHintsPosition position);

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