#include "Widgets.h"

namespace Widgets
{

    void formatTime(
        uint32_t milliseconds,
        char *buffer)
    {
        uint32_t totalSeconds =
            milliseconds / 1000;

        uint8_t minutes =
            totalSeconds / 60;

        uint8_t seconds =
            totalSeconds % 60;

        sprintf(
            buffer,
            "%02u:%02u",
            minutes,
            seconds);
    }

    String fitText(
        Adafruit_ST7789 &tft,
        const char *text,
        int maxWidth)
    {
        String result = text;

        int16_t x, y;
        uint16_t w, h;

        while (true)
        {
            tft.getTextBounds(
                result,
                0,
                0,
                &x,
                &y,
                &w,
                &h);

            if (w <= maxWidth)
                break;

            if (result.length() <= 3)
                break;

            result.remove(result.length() - 1);
        }

        if (result != text)
        {
            result += "...";
        }

        return result;
    }
}

void Widgets::drawProgressOutline(
    Adafruit_ST7789 &tft,
    int x,
    int y,
    int width,
    int height,
    uint16_t color)
{
    tft.drawRoundRect(
        x,
        y,
        width,
        height,
        4,
        color);
}

void Widgets::drawProgressFill(
    Adafruit_ST7789 &tft,
    int x,
    int y,
    int width,
    int height,
    uint32_t value,
    uint32_t maxValue,
    uint16_t color,
    uint16_t background)
{
    if (maxValue == 0)
        return;

    int innerWidth = width - 4;

    int fill =
        (value * innerWidth) /
        maxValue;

    if (fill > innerWidth)
        fill = innerWidth;

    tft.fillRect(
        x + 2,
        y + 2,
        innerWidth,
        height - 4,
        background);

    tft.fillRect(
        x + 2,
        y + 2,
        fill,
        height - 4,
        color);
}