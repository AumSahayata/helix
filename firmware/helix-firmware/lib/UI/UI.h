#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

namespace Theme
{
    constexpr uint16_t BACKGROUND = ST77XX_BLACK;
    constexpr uint16_t TEXT       = ST77XX_WHITE;

    constexpr uint16_t PRIMARY    = ST77XX_CYAN;
    constexpr uint16_t SUCCESS    = ST77XX_GREEN;
    constexpr uint16_t WARNING    = ST77XX_YELLOW;
    constexpr uint16_t MUTED      = 0x7BEF;
}

class UI
{
public:
    UI(Adafruit_ST7789 &display);

    void clear();

    void drawHeader(const char *title);

    void drawFooterHint(
        const char *left,
        const char *center,
        const char *right
    );

    void drawControls(
        const char *selectAction,
        const char *upAction,
        const char *backAction
    );

private:
    Adafruit_ST7789 &tft;
};