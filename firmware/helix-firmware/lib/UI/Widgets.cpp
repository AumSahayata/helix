#include "Widgets.h"

namespace
{
    constexpr int16_t kIconSize = 8;
    constexpr int16_t kIconHeight = 16;
    constexpr int16_t kBarWidth = 3;
    constexpr int16_t kGap = 4;
}

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

    fill = constrain(fill, 0, innerWidth);

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

void Widgets::drawIcon(
    Adafruit_ST7789 &tft,
    int16_t centerX,
    int16_t centerY,
    Icon icon,
    uint16_t color)
{
    switch (icon)
    {
    case Icon::Previous:
        drawPreviousIcon(tft, centerX, centerY, color);
        break;

    case Icon::Play:
        drawPlayIcon(tft, centerX, centerY, color);
        break;

    case Icon::Pause:
        drawPauseIcon(tft, centerX, centerY, color);
        break;

    case Icon::Next:
        drawNextIcon(tft, centerX, centerY, color);
        break;
    }
}


// ------------------------ MarqueeText Implementation ------------------------
namespace
{
    constexpr uint32_t kPauseDuration = 1000;
    constexpr uint32_t kScrollInterval = 75;
    constexpr int16_t kScrollPadding = 16;
}

void MarqueeText::setBounds(
    int16_t x,
    int16_t y,
    int16_t width)
{
    x_ = x;
    y_ = y;
    width_ = width;
}

void MarqueeText::reset()
{
    offset_ = 0;

    lastUpdate_ = millis();

    dirty_ = true;

    if (textWidth_ <= width_)
    {
        state_ = State::Idle;
    }
    else
    {
        state_ = State::PauseStart;
    }
}

bool MarqueeText::needsRedraw() const
{
    return dirty_;
}

void MarqueeText::setTextSize(uint8_t size)
{
    textSize_ = size;
}

void MarqueeText::setText(
    Adafruit_ST7789 &tft,
    const String &text)
{
    if (text == text_)
    {
        return;
    }

    text_ = text;

    tft.setTextSize(textSize_);

    tft.getTextBounds(
        text_,
        0,
        0,
        &textBX_,
        &textBY_,
        &textWidth_,
        &textHeight_);

    reset();
}

void MarqueeText::update()
{
    if (state_ == State::Idle)
    {
        return;
    }

    uint32_t now = millis();

    switch (state_)
    {
    case State::PauseStart:

        if (now - lastUpdate_ >= kPauseDuration)
        {
            state_ = State::Scroll;
            lastUpdate_ = now;
        }

        break;

    case State::Scroll:

        if (now - lastUpdate_ >= kScrollInterval)
        {
            ++offset_;
            dirty_ = true;
            lastUpdate_ = now;

            if (offset_ >= (textWidth_ - width_ + kScrollPadding))
            {
                state_ = State::PauseEnd;
                lastUpdate_ = now;
            }
        }

        break;

    case State::PauseEnd:

        if (now - lastUpdate_ >= kPauseDuration)
        {
            offset_ = 0;
            dirty_ = true;

            state_ = State::PauseStart;
            lastUpdate_ = now;
        }

        break;

    default:
        break;
    }
}

void MarqueeText::draw(
    Adafruit_ST7789 &tft,
    uint16_t color,
    uint16_t background)
{
    if (!dirty_)
    {
        return;
    }

    dirty_ = false;

    constexpr int16_t kTitleHeight = 18;

    tft.setTextSize(textSize_);
    tft.setTextColor(color, background);
    tft.setTextWrap(false);

    tft.fillRect(
        x_,
        y_,
        width_,
        kTitleHeight,
        background);

    int16_t drawX = x_ - offset_;

    for (size_t i = 0; i < text_.length(); ++i)
    {
        char c = text_[i];

        int16_t bx;
        int16_t by;
        uint16_t cw;
        uint16_t ch;

        char glyph[2] = {c, '\0'};

        tft.getTextBounds(
            glyph,
            0,
            0,
            &bx,
            &by,
            &cw,
            &ch);

        if (drawX + cw < x_)
        {
            drawX += cw;
            continue;
        }

        if (drawX > x_ + width_)
        {
            break;
        }

        tft.setCursor(drawX, y_);
        tft.print(glyph);

        drawX += cw;
    }
}