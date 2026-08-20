#include "Logo.h"

#include "Theme.h"

namespace
{
    constexpr float SCALE = 1.6f;
}

inline int16_t S(float value)
{
    return static_cast<int16_t>(value * SCALE);
}

void Logo::drawOutline(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    uint16_t color)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        drawEdge(
            tft,
            x,
            y,
            i,
            color);
    }
}

void Logo::drawAccent(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    uint16_t color)
{
    const int16_t hex2[6][2] =
        {
            {x + S(37), y + S(26)},
            {x + S(57), y + S(37)},
            {x + S(57), y + S(60)},
            {x + S(37), y + S(72)},
            {x + S(17), y + S(60)},
            {x + S(17), y + S(37)}};

    tft.fillTriangle(
        hex2[0][0], hex2[0][1],
        hex2[1][0], hex2[1][1],
        hex2[2][0], hex2[2][1],
        color);

    tft.fillTriangle(
        hex2[0][0], hex2[0][1],
        hex2[2][0], hex2[2][1],
        hex2[3][0], hex2[3][1],
        color);

    tft.fillTriangle(
        hex2[0][0], hex2[0][1],
        hex2[3][0], hex2[3][1],
        hex2[4][0], hex2[4][1],
        color);

    tft.fillTriangle(
        hex2[0][0], hex2[0][1],
        hex2[4][0], hex2[4][1],
        hex2[5][0], hex2[5][1],
        color);
}

void Logo::drawTriangle(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    uint8_t triangle)
{
    const int16_t hex2[6][2] =
        {
            {x + S(37), y + S(26)},
            {x + S(57), y + S(37)},
            {x + S(57), y + S(60)},
            {x + S(37), y + S(72)},
            {x + S(17), y + S(60)},
            {x + S(17), y + S(37)}};

    switch (triangle)
    {
    case 0:
        tft.fillTriangle(
            hex2[0][0], hex2[0][1],
            hex2[1][0], hex2[1][1],
            hex2[2][0], hex2[2][1],
            Theme::colors().primary);
        break;

    case 1:
        tft.fillTriangle(
            hex2[0][0], hex2[0][1],
            hex2[2][0], hex2[2][1],
            hex2[3][0], hex2[3][1],
            Theme::colors().primary);
        break;

    case 2:
        tft.fillTriangle(
            hex2[0][0], hex2[0][1],
            hex2[3][0], hex2[3][1],
            hex2[4][0], hex2[4][1],
            Theme::colors().primary);
        break;

    case 3:
        tft.fillTriangle(
            hex2[0][0], hex2[0][1],
            hex2[4][0], hex2[4][1],
            hex2[5][0], hex2[5][1],
            Theme::colors().primary);
        break;
    }
}

void Logo::draw(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    uint16_t color)
{
    drawOutline(tft, x, y, color);
    drawAccent(tft, x, y, color);
}

void Logo::drawEdge(
    Adafruit_ST7789 &tft,
    int16_t x,
    int16_t y,
    uint8_t edge,
    uint16_t color)
{
    const int16_t hex1[6][2] =
        {
            {x + S(20), y + S(0)},
            {x + S(40), y + S(11)},
            {x + S(40), y + S(34)},
            {x + S(20), y + S(46)},
            {x + S(0), y + S(34)},
            {x + S(0), y + S(11)}};

    if (edge >= 6)
        return;

    int next = (edge + 1) % 6;

    tft.drawLine(
        hex1[edge][0],
        hex1[edge][1],
        hex1[next][0],
        hex1[next][1],
        color);
}