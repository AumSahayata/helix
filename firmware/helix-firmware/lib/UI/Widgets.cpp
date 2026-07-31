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

void MarqueeText::draw(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    int16_t width,
    const String &text,
    uint16_t color,
    uint16_t background)
{
    if (text != currentText)
    {
        currentText = text;

        offset = 0;

        state = State::PauseStart;

        lastUpdate = millis();
    }

    int16_t bx, by;
    uint16_t textWidth, textHeight;

    tft.getTextBounds(
        text,
        0,
        0,
        &bx,
        &by,
        &textWidth,
        &textHeight);

    if (textWidth <= width)
    {
        tft.fillRect(
            x,
            y,
            width,
            textHeight + 2,
            background);

        tft.setCursor(x, y);
        tft.setTextColor(color);
        tft.print(text);

        return;
    }

    switch (state)
    {
    case State::PauseStart:

        if (millis() - lastUpdate >= 1000)
        {
            state = State::ScrollLeft;
            lastUpdate = millis();
        }

        break;

    case State::ScrollLeft:

        if (millis() - lastUpdate >= 30)
        {
            offset++;

            lastUpdate = millis();
        }

        break;

    default:
        break;
    }

    tft.fillRect(
        x,
        y,
        width,
        textHeight + 2,
        background);

    tft.setTextColor(color);

    tft.setCursor(x - offset, y);

    tft.print(text);
}

void Widgets::drawOverlayBox(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height)
{
    tft.fillRoundRect(
        x,
        y,
        width,
        height,
        8,
        ST77XX_BLACK);

    tft.drawRoundRect(
        x,
        y,
        width,
        height,
        8,
        ST77XX_WHITE);
}