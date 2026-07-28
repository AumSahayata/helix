#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

class Stopwatch
{
public:
    Stopwatch(Adafruit_ST7789 &display);

    void begin();

    void startPause();
    void reset();
    void update();

    void drawScreen();

    bool isRunning() const;

private:
    Adafruit_ST7789 &tft;

    bool running = false;

    unsigned long startTime = 0;
    unsigned long elapsed = 0;
    unsigned long lastDraw = 0;

    static constexpr unsigned long REFRESH_MS = 50;

    void drawTime(unsigned long time);
};