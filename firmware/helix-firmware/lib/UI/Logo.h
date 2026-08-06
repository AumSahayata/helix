#pragma once

#include <Adafruit_ST7789.h>

namespace Logo
{
    void drawOutline(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y);

    void drawAccent(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y);

    void draw(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y);

    void drawTriangle(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint8_t triangle);

    void drawEdge(
        Adafruit_ST7789 &tft,
        int16_t x,
        int16_t y,
        uint8_t edge);
}