#include "UI.h"

UI::UI(Adafruit_ST7789& display)
    : tft(display)
{
}

void UI::clear()
{
    tft.fillScreen(Theme::BACKGROUND);
}

void UI::drawHeader(const char* title)
{
    tft.setTextColor(Theme::PRIMARY);
    tft.setTextSize(2);

    // Brand
    tft.setCursor(10, 15);
    tft.print("HELIX");

    // Screen title
    tft.setTextSize(1);
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
        &h
    );

    // Right-align title
    tft.setCursor(230 - w, 20);
    tft.print(title);

    // Separator
    tft.drawFastHLine(
        10,
        45,
        220,
        Theme::PRIMARY
    );
}

void UI::drawMenuItem(
    int index,
    const char *text,
    bool selected)
{
    constexpr int16_t kMenuStartY = 75;
    constexpr int16_t kMenuItemSpacing = 35;

    int y = kMenuStartY + (index * kMenuItemSpacing);

    // Clear only this menu row
    tft.fillRect(
        10,
        y - 7,
        220,
        30,
        Theme::BACKGROUND);

    tft.setTextSize(2);

    if (selected)
    {
        tft.fillRoundRect(
            10,
            y - 7,
            220,
            30,
            5,
            Theme::PRIMARY);

        tft.setTextColor(Theme::BACKGROUND);
        tft.setCursor(25, y);
        tft.print("> ");
        tft.print(text);
    }
    else
    {
        tft.setTextColor(Theme::TEXT);
        tft.setCursor(25, y);
        tft.print("  ");
        tft.print(text);
    }
}