#include "UI.h"
#include "App.h"
#include "Widgets.h"

UI::UI(Adafruit_ST7789 &display)
    : tft(display)
{
}

void UI::clear()
{
    tft.fillScreen(Theme::colors().background);
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
    tft.setTextColor(Theme::colors().primary);

    tft.setCursor(
        kPadding,
        kBrandY);

    tft.print("HELIX");

    // ---------- Title ----------
    tft.setTextSize(kTitleSize);
    tft.setTextColor(Theme::colors().muted);

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
        Theme::colors().primary);
}

void UI::drawMenuItem(
    int index,
    const String &text,
    bool selected,
    Icons::Type icon)
{
    constexpr int16_t kPadding = 10;
    constexpr int16_t kStartY = 75;
    constexpr int16_t kItemSpacing = 35;
    constexpr int16_t kItemVerticalOffset = 7;
    constexpr int16_t kItemHeight = 30;
    constexpr int16_t kCornerRadius = 5;
    constexpr int16_t kTextSize = 2;

    constexpr int16_t kIconX = 24;
    constexpr int16_t kTextX = 42;

    const int16_t y = kStartY + (index * kItemSpacing);
    const int16_t itemWidth = tft.width() - (kPadding * 2);
    const int16_t centerY = y + (kItemHeight / 2) - kItemVerticalOffset;

    const uint16_t bgColor =
        selected ? Theme::colors().primary : Theme::colors().background;

    const uint16_t textColor =
        selected ? Theme::colors().background : Theme::colors().text;

    // Clear row
    tft.fillRect(
        kPadding,
        y - kItemVerticalOffset,
        itemWidth,
        kItemHeight,
        Theme::colors().background);

    if (selected)
    {
        tft.fillRoundRect(
            kPadding,
            y - kItemVerticalOffset,
            itemWidth,
            kItemHeight,
            kCornerRadius,
            Theme::colors().primary);
    }

    if (icon != Icons::Type::None)
    {
        Icons::draw(
            tft,
            kIconX,
            centerY,
            icon,
            textColor);
    }

    tft.setTextSize(kTextSize);
    tft.setTextColor(textColor);
    tft.setCursor(kTextX, y);
    tft.print(text);
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

void UI::drawControlHints(
    const ControlHint *hints,
    size_t count,
    ControlHintsPosition position)
{
    if (hints == nullptr || count == 0)
    {
        return;
    }

    constexpr int16_t kOverlayWidth = 230;
    constexpr int16_t kPadding = 8;
    constexpr int16_t kHeaderHeight = 26;
    constexpr int16_t kRowHeight = 20;

    constexpr uint8_t kHeaderTextSize = 2;
    constexpr uint8_t kTextSize = 1;

    const int16_t overlayHeight =
        kPadding +
        kHeaderHeight +
        (count * kRowHeight) +
        kPadding;

    const int16_t x = centerX(kOverlayWidth);

    const int16_t centerYPosition  = centerY(overlayHeight);
    int16_t y = centerYPosition;

    switch (position)
    {
    case ControlHintsPosition::Top:
        y = centerY(overlayHeight) - 30;
        break;

    case ControlHintsPosition::Bottom:
        y = tft.height() - overlayHeight - 30;
        break;

    case ControlHintsPosition::Center:
    default:
        break;
    }

    Widgets::drawOverlayBox(
        tft,
        x,
        y,
        kOverlayWidth,
        overlayHeight);

    // Header
    tft.setTextSize(kHeaderTextSize);
    tft.setTextColor(Theme::colors().primary);
    tft.setCursor(
        x + kPadding,
        y + kPadding);

    tft.print("CONTROLS");

    tft.drawFastHLine(
        x + kPadding,
        y + kHeaderHeight,
        kOverlayWidth - (kPadding * 2),
        Theme::colors().muted);

    // Columns
    constexpr int16_t kButtonOffset = 8;
    constexpr int16_t kGestureOffset = 62;
    constexpr int16_t kActionOffset = 112;

    tft.setTextSize(kTextSize);
    tft.setTextColor(Theme::colors().text);

    for (size_t i = 0; i < count; ++i)
    {
        const int16_t rowY =
            y +
            kHeaderHeight +
            9 +
            (i * kRowHeight);

        tft.setCursor(x + kButtonOffset, rowY);
        tft.print(hints[i].button);

        tft.setCursor(x + kGestureOffset, rowY);
        tft.print(hints[i].gesture);

        tft.setCursor(x + kActionOffset, rowY);
        tft.print(hints[i].action);
    }
}