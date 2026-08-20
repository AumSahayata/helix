#pragma once

#include <Adafruit_ST7789.h>
#include <UI.h>

namespace Logo
{
    void drawOutline(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint16_t color = Theme::TEXT);

    void drawAccent(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint16_t color = Theme::PRIMARY);

    void draw(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint16_t color = Theme::PRIMARY);

    void drawTriangle(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint8_t triangle);

    void drawEdge(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint8_t edge,
        uint16_t color = Theme::TEXT);
}