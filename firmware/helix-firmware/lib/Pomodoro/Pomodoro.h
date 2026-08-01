#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <App.h>
#include <UI.h>
#include <Input.h>

class Pomodoro: public App
{
public:
    Pomodoro(Adafruit_ST7789 &display, UI &ui);

    void startPause();
    void reset();

    bool isRunning() const;

    void begin() override;
    void draw() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    enum class Mode
    {
        FOCUS,
        BREAK
    };

    Adafruit_ST7789 &tft;
    UI &ui;

    Mode mode = Mode::FOCUS;

    bool running = false;

    unsigned long startTime = 0;
    unsigned long remaining = 0;
    unsigned long lastDraw = 0;

    static constexpr unsigned long REFRESH_MS = 200;

    static constexpr unsigned long FOCUS_TIME =
        25UL * 60UL * 1000UL;

    static constexpr unsigned long BREAK_TIME =
        5UL * 60UL * 1000UL;

    void drawTime(unsigned long timeRemaining);
    void drawMode();
    void drawStatus();

    void switchMode();
};