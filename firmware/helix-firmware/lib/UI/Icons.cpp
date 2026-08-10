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

        default:
            break;
        }
    }
}
