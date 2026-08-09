#include "UI.h"

UI::UI(Adafruit_ST7789 &display)
    : tft(display)
{
}

void UI::clear()
{
    tft.fillScreen(Theme::BACKGROUND);
}

void UI::drawHeader(const char *title)
{
    constexpr int16_t kPadding = 10;

    constexpr int16_t kBrandY = 15;
    constexpr int16_t kTitleY = 20;

    constexpr int16_t kDividerY = 45;

    constexpr uint8_t kBrandSize = 2;
    constexpr uint8_t kTitleSize = 1;

    // ---------- Brand ----------
    tft.setTextSize(kBrandSize);
    tft.setTextColor(Theme::PRIMARY);

    tft.setCursor(
        kPadding,
        kBrandY);

    tft.print("HELIX");

    // ---------- Title ----------
    tft.setTextSize(kTitleSize);
    tft.setTextColor(Theme::MUTED);

    int16_t x1, y1;
    uint16_t w, h;

    tft.getTextBounds(
        title,
        0,
        0,
        &x1,
        &y1,
        &w,
        &h);

    tft.setCursor(
        tft.width() - kPadding - w,
        kTitleY);

    tft.print(title);

    // ---------- Divider ----------
    tft.drawFastHLine(
        kPadding,
        kDividerY,
        tft.width() - (kPadding * 2),
        Theme::PRIMARY);
}

void UI::drawMenuItem(
    int index,
    const char *text,
    bool selected)
{
    constexpr int16_t kPadding = 10;

    constexpr int16_t kStartY = 75;
    constexpr int16_t kItemSpacing = 35;
    constexpr int16_t kItemVerticalOffset = 7;

    constexpr int16_t kItemHeight = 30;
    constexpr int16_t kCornerRadius = 5;

    constexpr int16_t kTextPadding = 15;

    constexpr uint8_t kTextSize = 2;

    const int16_t y = kStartY + (index * kItemSpacing);

    const int16_t itemWidth = tft.width() - (kPadding * 2);

    // Clear row
    tft.fillRect(
        kPadding,
        y - kItemVerticalOffset,
        itemWidth,
        kItemHeight,
        Theme::BACKGROUND);

    tft.setTextSize(kTextSize);

    if (selected)
    {
        tft.fillRoundRect(
            kPadding,
            y - kItemVerticalOffset,
            itemWidth,
            kItemHeight,
            kCornerRadius,
            Theme::PRIMARY);

        tft.setTextColor(Theme::BACKGROUND);

        tft.setCursor(
            kPadding + kTextPadding,
            y);

        tft.print("> ");
        tft.print(text);
    }
    else
    {
        tft.setTextColor(Theme::TEXT);

        tft.setCursor(
            kPadding + kTextPadding,
            y);

        tft.print("  ");
        tft.print(text);
    }
}

int16_t UI::centerX(int16_t width) const
{
    return (tft.width() - width) / 2;
}

int16_t UI::centerY(int16_t height) const
{
    return (tft.height() - height) / 2;
}

UI::Position UI::centerRect(
    int16_t width,
    int16_t height) const
{
    return {
        centerX(width),
        centerY(height)};
}

UI::Position UI::centeredTextPosition(
    const String &text,
    int16_t centerY,
    uint8_t size,
    bool centerVertically) const
{
    int16_t x1, y1;
    uint16_t w, h;

    tft.setTextSize(size);

    tft.getTextBounds(
        text,
        0,
        0,
        &x1,
        &y1,
        &w,
        &h);

    Position pos;

    pos.x = centerX(w);

    if (centerVertically)
    {
        pos.y = centerY - (h / 2);
    }
    else
    {
        pos.y = centerY;
    }

    return pos;
}

void UI::drawCenteredText(
    const String &text,
    int16_t centerY,
    uint8_t size,
    uint16_t color,
    bool centerVertically) const
{
    Position pos = centeredTextPosition(
        text,
        centerY,
        size,
        centerVertically);

    tft.setTextSize(size);
    tft.setTextColor(color);
    tft.setCursor(pos.x, pos.y);
    tft.print(text);
}

int16_t UI::textHeight(uint8_t size) const
{
    int16_t x1, y1;
    uint16_t w, h;

    tft.setTextSize(size);

    tft.getTextBounds(
        "Ag",
        0,
        0,
        &x1,
        &y1,
        &w,
        &h);

    return h;
}

int16_t UI::nextLine(
    VerticalLayout &layout,
    uint8_t textSize,
    int16_t gap) const
{
    int16_t y = layout.currentY;

    layout.currentY += textHeight(textSize) + gap;

    return y;
}

int16_t UI::nextBlock(
    VerticalLayout &layout,
    int16_t blockHeight,
    int16_t gap) const
{
    int16_t y = layout.currentY;

    layout.currentY += blockHeight + gap;

    return y;
}