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

void UI::drawFooterHint(
    const char* left,
    const char* center,
    const char* right
)
{
    tft.setTextSize(1);
    tft.setTextColor(Theme::MUTED);

    tft.setCursor(10, 295);
    tft.print(left);

    tft.setCursor(95, 295);
    tft.print(center);

    tft.setCursor(190, 295);
    tft.print(right);
}

void UI::drawControls(
    const char* selectAction,
    const char* upAction,
    const char* backAction
)
{
    tft.setTextSize(1);

    // SELECT
    tft.setTextColor(Theme::PRIMARY);
    tft.setCursor(20, 220);
    tft.print("SELECT: ");

    tft.setTextColor(Theme::TEXT);
    tft.print(" ");
    tft.print(selectAction);

    // UP
    tft.setTextColor(Theme::PRIMARY);
    tft.setCursor(20, 245);
    tft.print("UP:");

    tft.setTextColor(Theme::TEXT);
    tft.print(" ");
    tft.print(upAction);

    // BACK
    tft.setTextColor(Theme::PRIMARY);
    tft.setCursor(20, 270);
    tft.print("BACK:");

    tft.setTextColor(Theme::TEXT);
    tft.print(" ");
    tft.print(backAction);
}