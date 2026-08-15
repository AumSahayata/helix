#include "Icons.h"

namespace Icons
{
    static void drawPlayIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {

        tft.fillTriangle(
            centerX - kIconSize / 2,
            centerY - kIconSize,
            centerX - kIconSize / 2,
            centerY + kIconSize,
            centerX + kIconSize,
            centerY,
            color);
    }

    static void drawPauseIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        tft.fillRect(
            centerX - kGap - kBarWidth,
            centerY - kIconHeight / 2,
            kBarWidth,
            kIconHeight,
            color);

        tft.fillRect(
            centerX + kGap,
            centerY - kIconHeight / 2,
            kBarWidth,
            kIconHeight,
            color);
    }

    static void drawPreviousIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        // Triangle (pointing left)
        tft.fillTriangle(
            centerX + kIconSize / 2,
            centerY - kIconSize,
            centerX + kIconSize / 2,
            centerY + kIconSize,
            centerX - kIconSize / 2,
            centerY,
            color);

        // Left bar
        tft.fillRect(
            centerX - kIconSize - kBarWidth,
            centerY - kIconHeight / 2,
            kBarWidth,
            kIconHeight,
            color);
    }

    static void drawNextIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        // Triangle
        tft.fillTriangle(
            centerX - kIconSize / 2,
            centerY - kIconSize,
            centerX - kIconSize / 2,
            centerY + kIconSize,
            centerX + kIconSize / 2,
            centerY,
            color);

        // Right bar
        tft.fillRect(
            centerX + kIconSize,
            centerY - kIconHeight / 2,
            kBarWidth,
            kIconHeight,
            color);
    }

    static void drawMusicIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        // Note head
        tft.fillCircle(
            centerX - 2,
            centerY + kIconSize / 2,
            3,
            color);

        // Stem
        tft.fillRect(
            centerX + 1,
            centerY - kIconHeight / 2,
            3,
            kIconHeight - 3,
            color);

        // Flag
        tft.drawFastHLine(
            centerX + 1,
            centerY - kIconHeight / 2,
            kIconSize,
            color);

        tft.drawFastHLine(
            centerX + 1,
            centerY - kIconHeight / 2 + 1,
            kIconSize - 2,
            color);
    }

    static void drawRandomIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        constexpr int16_t halfWidth = kIconSize;
        constexpr int16_t halfHeight = kIconHeight / 2;
        constexpr int16_t arrowSize = kGap;

        const int16_t left = centerX - halfWidth;
        const int16_t right = centerX + halfWidth;
        const int16_t top = centerY - halfHeight;
        const int16_t bottom = centerY + halfHeight;

        // Upper diagonal
        tft.drawLine(
            left,
            bottom,
            right,
            top,
            color);
        tft.drawLine(
            left,
            bottom - 1,
            right - 1,
            top,
            color);

        // Lower diagonal
        tft.drawLine(
            left,
            top,
            right,
            bottom,
            color);
        tft.drawLine(
            left,
            top + 1,
            right - 1,
            bottom,
            color);

        // Top arrow
        tft.fillTriangle(
            right,
            top,
            right - arrowSize,
            top,
            right,
            top + arrowSize,
            color);

        // Bottom arrow
        tft.fillTriangle(
            right,
            bottom,
            right - arrowSize,
            bottom,
            right,
            bottom - arrowSize,
            color);
    }

    static void drawSettingsIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        // Wrench: open jaw (two prongs) at top-left, diagonal handle,
        // rounded closed end at bottom-right. All additive, no bg needed.

        int16_t jawX = centerX - kIconSize / 2;
        int16_t jawY = centerY - kIconHeight / 2;

        int16_t prongLen = kIconSize; // length of each jaw prong
        int16_t jawGap = kGap;        // opening width between prongs

        // Upper prong (angled outward)
        tft.drawLine(jawX, jawY, jawX + prongLen, jawY + prongLen / 2, color);
        tft.drawLine(jawX, jawY + 1, jawX + prongLen, jawY + prongLen / 2 + 1, color);

        // Lower prong (angled outward, offset by jawGap to form the opening)
        tft.drawLine(jawX, jawY + jawGap, jawX + prongLen - 2, jawY + jawGap + prongLen / 2 - 2, color);
        tft.drawLine(jawX, jawY + jawGap + 1, jawX + prongLen - 2, jawY + jawGap + prongLen / 2 - 1, color);

        // Base of jaw connecting both prongs (short vertical joint)
        tft.drawFastVLine(jawX, jawY, jawGap + 1, color);

        // Diagonal handle from jaw base down to the closed end
        int16_t handleStartX = jawX + prongLen - 2;
        int16_t handleStartY = jawY + jawGap + prongLen / 2 - 1;
        int16_t endX = centerX + kIconSize / 2;
        int16_t endY = centerY + kIconHeight / 2;

        for (int16_t o = 0; o < kBarWidth; ++o)
        {
            tft.drawLine(handleStartX, handleStartY + o, endX, endY + o, color);
        }

        // Closed rounded end cap
        tft.fillCircle(endX, endY - kBarWidth / 2, kBarWidth / 2 + 1, color);
    }

    static void drawDisplayIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        constexpr int16_t screenWidth = kIconSize * 2;
        constexpr int16_t screenHeight = kIconHeight - 6;

        const int16_t x = centerX - screenWidth / 2;
        const int16_t y = centerY - screenHeight / 2;

        tft.drawRoundRect(
            x,
            y,
            screenWidth,
            screenHeight,
            2,
            color);

        // Stand
        tft.drawFastVLine(
            centerX,
            y + screenHeight,
            kGap - 1,
            color);

        // Base
        tft.drawFastHLine(
            centerX - kGap,
            y + screenHeight + kGap - 1,
            kGap * 2,
            color);
    }

    static void drawInfoIcon(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        uint16_t color)
    {
        tft.drawCircle(
            centerX,
            centerY,
            kIconSize - 1,
            color);

        tft.fillCircle(
            centerX,
            centerY - kGap,
            1,
            color);

        tft.drawFastVLine(
            centerX,
            centerY - 1,
            kGap + 2,
            color);
    }

    void draw(
        Adafruit_ST7789 &tft,
        int16_t centerX,
        int16_t centerY,
        Type icon,
        uint16_t color)
    {
        switch (icon)
        {
        case Type::None:
            break;

        case Type::Previous:
            drawPreviousIcon(tft, centerX, centerY, color);
            break;

        case Type::Play:
            drawPlayIcon(tft, centerX, centerY, color);
            break;

        case Type::Pause:
            drawPauseIcon(tft, centerX, centerY, color);
            break;

        case Type::Next:
            drawNextIcon(tft, centerX, centerY, color);
            break;

        case Type::Random:
            drawRandomIcon(tft, centerX, centerY, color);
            break;

        case Type::Music:
            drawMusicIcon(tft, centerX, centerY, color);
            break;

        case Type::Display:
            drawDisplayIcon(tft, centerX, centerY, color);
            break;

        case Type::Settings:
            drawSettingsIcon(tft, centerX, centerY, color);
            break;

        case Type::Info:
            drawInfoIcon(tft, centerX, centerY, color);
            break;

        default:
            break;
        }
    }
}
