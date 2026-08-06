#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <App.h>
#include <UI.h>
#include <Input.h>

class Stopwatch : public App
{
public:
    Stopwatch(Adafruit_ST7789 &display, UI &ui);

    void begin();

    void startPause();
    void reset();

    bool isRunning() const;

    void draw() override;
    void update() override;
    bool handleInput(InputEvent event) override;

private:
    Adafruit_ST7789 &tft;
    UI &ui;

    bool running = false;

    unsigned long startTime = 0;
    unsigned long elapsed = 0;
    unsigned long lastDraw = 0;

    static constexpr unsigned long REFRESH_MS = 50;

    void drawTime(unsigned long time);
};